#include "proxerapp.h"
#include <QtRestClient>
#include "core.h"
#include "coremessage.h"
#include "animeinfo.h"
#include "proxer-api-key.h"

REGISTER_CORE_APP(ProxerApp)

ProxerApp::ProxerApp(QObject *parent) :
	CoreApp(parent),
	store(nullptr),
	loader(nullptr),
	mainControl(nullptr),
	statusControl(nullptr),
	passiveUpdate(false)
{
	qRegisterMetaType<AnimeList>();
	QtRestClient::registerListConverters<ProxerEntryData*>();
}

void ProxerApp::checkForSeasonUpdates()
{
	mainControl->updateLoadStatus(true);
	loader->checkForUpdates(store->animeInfoList());
}

void ProxerApp::showMainControl()
{
	showControl(mainControl);
	if(statusControl)
		closeControl(statusControl);
}

void ProxerApp::setupParser(QCommandLineParser &parser, bool &allowInvalid)
{
	CoreApp::setupParser(parser, allowInvalid);
	parser.addOption({
						 {"u", "update"},
						 "SeasonProxer will start with a GUI, checking for updates passively"
					 });
}

bool ProxerApp::startApp(const QCommandLineParser &parser)
{
	if(autoShowHelpOrVersion(parser))
		return true;

	//create proxer rest api
	auto client = new QtRestClient::RestClient(qApp);
	client->setBaseUrl(QStringLiteral("https://proxer.me/api"));
	client->setApiVersion({1});
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);
	client->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling
	QtRestClient::RestClient::addGlobalApi(Core::ProxerRest, client);

	//anime data store
	store = new AnimeStore(this);
	connect(store, &AnimeStore::storeLoaded,
			this, &ProxerApp::storeLoaded,
			Qt::QueuedConnection);

	loader = new SeasonStatusLoader(this);
	connect(loader, &SeasonStatusLoader::newSeasonsDetected,
			store, &AnimeStore::saveAnime);

	mainControl = new MainControl(store, this);
	connect(loader, &SeasonStatusLoader::updated,
			mainControl, &MainControl::setProgress);
	connect(loader, &SeasonStatusLoader::completed,
			this, &ProxerApp::updateDone);

	passiveUpdate = parser.isSet("update");
	if(!passiveUpdate)
		showControl(mainControl);
	return true;
}

void ProxerApp::aboutToQuit()
{

}

void ProxerApp::storeLoaded()
{
	if(passiveUpdate)
		loader->checkForUpdates(store->animeInfoList());
	else
		mainControl->updateLoadStatus(false);
}

void ProxerApp::updateDone(bool hasUpdates, QString errorString)
{
	mainControl->updateLoadStatus(false);

	if(passiveUpdate) {
		passiveUpdate = false;
		if(hasUpdates || !errorString.isNull()) {
			statusControl = new StatusControl(this);
			if(errorString.isNull())
				statusControl->loadUpdateStatus(store->animeInfoList());
			else
				statusControl->loadErrorStatus(errorString);
			showControl(statusControl);
		} else
			qApp->quit();
	} else {
		if(!errorString.isNull())
			CoreMessage::critical(tr("Season check failed"), errorString);
		else if(hasUpdates)
			CoreMessage::information(tr("Season check completed"), tr("New Seasons are available!"));
		else
			CoreMessage::information(tr("Season check completed"), tr("No seasons changed."));
	}
}
