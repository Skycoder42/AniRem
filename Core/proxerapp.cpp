#include "proxerapp.h"
#include <QtRestClient>
#include <QDate>
#include <QTranslator>
#include <QLibraryInfo>
#include <wsauthenticator.h>
#include <exceptions.h>
#include "coremessage.h"
#include "animeinfo.h"
#include "apikeys.h"
#include "jsonserializer.h"
#include "proxerapi.h"
#include "seasonstatusloader.h"
#include "maincontrol.h"
#include "statuscontrol.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

ProxerApp::ProxerApp(QObject *parent) :
	CoreApp(parent),
	store(nullptr),
	loader(nullptr),
	mainControl(nullptr),
	statusControl(nullptr),
	passiveUpdate(false),
	showNoUpdatesInfo(false)
{
	Q_INIT_RESOURCE(seasonproxer_core);

	qRegisterMetaType<QMap<AnimeInfo::SeasonType, AnimeInfo::SeasonInfo>>("QMap<AnimeInfo::SeasonType, AnimeInfo::SeasonInfo>");
	QJsonSerializer::registerListConverters<AnimeInfo*>();

	//load translations
	auto translator = new QTranslator(this);
	if(translator->load(QLocale(),
						QStringLiteral("seasonproxer"),
						QStringLiteral("_"),
						QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		qApp->installTranslator(translator);
	else {
		qWarning() << "Failed to load translations";
		delete translator;
	}
}

bool ProxerApp::isUpdater() const
{
	return passiveUpdate;
}

void ProxerApp::checkForSeasonUpdate(AnimeInfo *info)
{
	mainControl->updateLoadStatus(true);
	loader->checkForUpdates({info});
}

void ProxerApp::checkForSeasonUpdates()
{
	mainControl->updateLoadStatus(true);
	showNoUpdatesInfo = true;
	loader->checkForUpdates(store->loadAll());
}

void ProxerApp::showMainControl()
{
	showControl(mainControl);
	if(passiveUpdate) {
		passiveUpdate = false;
		closeControl(statusControl);
	}
}

void ProxerApp::quitApp()
{
#ifdef Q_OS_ANDROID
	auto service = QtAndroid::androidService();
	if(service.isValid())
		service.callMethod<void>("stopSelf");
	else
#endif
	qApp->quit();
}

void ProxerApp::setupParser(QCommandLineParser &parser, bool &allowInvalid) const
{
	CoreApp::setupParser(parser, allowInvalid);
	parser.addOption({
						 {"u", "update"},
						 "AniRem will start with a GUI, checking for updates passively"
					 });
}

bool ProxerApp::startApp(const QCommandLineParser &parser)
{
	if(autoShowHelpOrVersion(parser))
		return true;

	//datasync setup
	try {
		QtDataSync::Setup()
				.setSerializer(new JsonSerializer())
				.create();
	} catch(QtDataSync::SetupLockedException &) {
		qInfo() << "Another instance is already running and blocking the setup."
				<< "Update check will be skipped!";
		qApp->quit();
		return true;
	} catch(QException &e) {
		qCritical() << "Failed to create setup with error:" << e.what();
		return false;
	}

	auto auth = QtDataSync::Setup::authenticatorForSetup<QtDataSync::WsAuthenticator>(this);
#ifndef QT_NO_DEBUG
	auth->setServerSecret(QStringLiteral("baum42"));
	auth->setRemoteUrl(QStringLiteral("ws://192.168.179.20:8080"));
#else
	auth->setServerSecret(DATASYNC_SERVER_SECRET);
	auth->setRemoteUrl(QStringLiteral("wss://apps.skycoder42.de/seasonproxer/"));
#endif
	auth->reconnect();
	auth->deleteLater();

	//anime data store
	store = new AnimeStore(this);
	connect(store, &AnimeStore::storeLoaded,
			this, &ProxerApp::storeLoaded,
			Qt::QueuedConnection);

	//fixup with additional setup for proxer rest api
	ProxerApi api;
	api.restClient()->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling

	//updater
	loader = new SeasonStatusLoader(this);
	connect(loader, &SeasonStatusLoader::animeInfoUpdated,
			store, &AnimeStore::save);

	//main control
	mainControl = new MainControl(store, this);
	connect(loader, &SeasonStatusLoader::updated,
			mainControl, &MainControl::setProgress);
	connect(loader, &SeasonStatusLoader::completed,
			this, &ProxerApp::updateDone);

	//init control flow
	passiveUpdate = parser.isSet("update");
	if(passiveUpdate) {
		statusControl = new StatusControl(this);
		connect(loader, &SeasonStatusLoader::updated,
				statusControl, &StatusControl::updateProgress);
		showControl(statusControl);
	} else
		showControl(mainControl);
	return true;
}

void ProxerApp::storeLoaded()
{
	if(passiveUpdate)
		syncLocalData(true);
	else
		mainControl->updateLoadStatus(false);
}

void ProxerApp::updateDone(bool hasUpdates, QString errorString)
{
	mainControl->updateLoadStatus(false);

	if(passiveUpdate) {
		if(hasUpdates || !errorString.isNull()) {
			if(errorString.isNull())
				statusControl->loadUpdateStatus(store->loadAll());
			else
				statusControl->loadErrorStatus(errorString);
		} else
			syncLocalData(false);
	} else {
		if(!errorString.isNull())
			CoreMessage::critical(tr("Season check failed"), errorString);
		else if(hasUpdates)
			CoreMessage::information(tr("Season check completed"), tr("New Seasons are available!"));
		else if(showNoUpdatesInfo) {
			showNoUpdatesInfo = false;
			CoreMessage::information(tr("Season check completed"), tr("No seasons changed."));
		}
	}
}

void ProxerApp::syncLocalData(bool updateNext)
{
	auto controller = new QtDataSync::SyncController(this);
	qDebug() << "Syncing local anime store...";
	auto resFn = [this, controller, updateNext](QtDataSync::SyncController::SyncState state) {
		//No matter how the result, try to check for updates anyway
		qInfo() << "Synced anime store with result:" << state;
		if(updateNext)
			automaticUpdateCheck();
		else
			quitApp();
		controller->deleteLater();
	};

	if(controller->syncState() == QtDataSync::SyncController::Disconnected)
		resFn(QtDataSync::SyncController::Disconnected);
	else
		controller->triggerSyncWithResult(resFn);
}

void ProxerApp::automaticUpdateCheck()
{
	QSettings settings;
	settings.beginGroup("updates");

	auto updateList = store->loadAll();

	//check if there are still animes with updates
	bool hasUpdates = false;
	foreach (auto info, updateList) {
		if(info->hasNewSeasons()) {
			hasUpdates = true;
			break;
		}
	}

	//sort by last update check time
	std::sort(updateList.begin(), updateList.end(), [](AnimeInfo *left, AnimeInfo *right){
		return left->lastUpdateCheck() < right->lastUpdateCheck();
	});

	//reduce to allowed list size
	auto maxSize = settings.value("checkLimit", 10).toInt();
	if(maxSize > 0)
		updateList = updateList.mid(0, maxSize);

	//reduce to allowed interval
	auto interval = settings.value("autoCheck", 7).toInt();
	for(auto i = 0; i < updateList.size(); i++) {
		if(updateList[i]->lastUpdateCheck().daysTo(QDate::currentDate()) < interval) {
			updateList = updateList.mid(0, i);
			break;
		}
	}

	//quit if none have to be updated or show already updated
	if(updateList.isEmpty() || interval == 0) {
		if(hasUpdates)
			updateDone(true, {});
		else {
			qInfo() << "No animes need season checks";
			quitApp();
		}
		return;
	}

	//begin the update
	qInfo() << "checking for updates of" << updateList.size() << "animes";
	loader->checkForUpdates(updateList, hasUpdates);

	settings.endGroup();
}
