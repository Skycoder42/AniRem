#include "addanimecontrol.h"
#include "maincontrol.h"
#include "proxerapp.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>
#include <coremessage.h>
#include <QTimer>
#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

MainControl::MainControl(AnimeStore *store, QObject *parent) :
	Control(parent),
	store(store),
	syncController(new QtDataSync::SyncController(this)),
	model(new QGenericListModel<AnimeInfo>(false, this)),
	_loading(true),
	detailsControl(new DetailsControl(store, this)),
	settingsControl(new ProxerSettingsControl(this))
{
	//TODO use async store instead of caching and make object model the data owner
	//TODO only update items on data change, don't recreate them
	connect(store, &AnimeStore::storeLoaded,
			this, &MainControl::storeListLoaded);
	connect(store, &AnimeStore::dataChanged,
			this, &MainControl::storeListLoaded);
	connect(store, &AnimeStore::dataResetted,
			this, &MainControl::storeListLoaded);
}

QGenericListModel<AnimeInfo> *MainControl::animeModel() const
{
	return model;
}

bool MainControl::isReloadingAnimes() const
{
	return _loading;
}

void MainControl::updateLoadStatus(bool loading)
{
	if(_loading == loading)
		return;

	_loading = loading;
	emit reloadingAnimesChanged(loading);
}

void MainControl::reload()
{
	coreApp->checkForSeasonUpdates();
}

void MainControl::showSettings()
{
	settingsControl->show();
}

void MainControl::showAbout()
{
	CoreMessage::about(tr("SeaonsProxer is an utility to easily get notifications for new seasons "
						  "of your favorite animes. You can add your animes and it will automatically "
						  "check for new seasons and notify you about them!"),
					   true,
					   QStringLiteral("https://skycoder42.de"));
}

void MainControl::resyncData()
{
	syncController->triggerResyncWithResult([this](QtDataSync::SyncController::SyncState state){
		if(state == QtDataSync::SyncController::Synced)
			CoreMessage::information(tr("Synchronization"), tr("Synchonization completed successfully!"));
		else
			CoreMessage::critical(tr("Synchronization"), tr("Failed to synchronize with server!"));
	});
}

void MainControl::uncheckAnime(int id)
{
	auto info = infoFromId(id);
	if(info && info->hasNewSeasons()) {
		info->setAllUnchanged();
		store->save(info);
	}
}

void MainControl::itemAction(int id)
{
	auto info = infoFromId(id);
	if(info) {
		if(settingsControl->uncheckEntries() && info->hasNewSeasons()) {
			info->setAllUnchanged();
			store->save(info);
		}
		if(settingsControl->openEntries())
			info->openUrl();
	}
}

void MainControl::addAnime()
{
	createAddControl();
}

void MainControl::addAnimeFromClipboard()
{
	auto clipBoard = QGuiApplication::clipboard();
	auto text = clipBoard->text();

	auto id = -1;
	auto url = QUrl::fromUserInput(text);
	if(url.isValid() && !url.isRelative()) {
		if(url.host() == QStringLiteral("proxer.me")) {
			QStringList pathElements = url.path().split("/", QString::SkipEmptyParts);
			if(pathElements.size() >= 2) {
				bool ok = false;
				id = pathElements[1].toInt(&ok);
				if(!ok)
					id = -1;
			}
		}
	}

	if(id == -1) {
		bool ok = false;
		id = text.toInt(&ok);
		if(!ok)
			id = -1;
	}

	if(id != -1)
		createAddControl(id);
	else
		showStatus(tr("Clipboard does not contain a proxer url or an id"));
}

void MainControl::showDetails(int id)
{
	auto info = infoFromId(id);
	if(info) {
		detailsControl->setAnimeInfo(info);
		detailsControl->show();
	} else {
		detailsControl->close();
		detailsControl->setAnimeInfo(nullptr);
	}
}

void MainControl::removeAnime(int id)
{
	auto info = infoFromId(id);
	if(info) {
		model->removeObject(model->index(info));
		showStatus(tr("Removed Anime: %1").arg(info->title()));
		store->remove(info->id());
	}
}

void MainControl::onShow()
{
	settingsControl->ensureAutoStart();
#ifdef Q_OS_ANDROID
	static bool once = true;
	if(once) {
		QtAndroid::hideSplashScreen();
		once = false;
	}
#endif
}

void MainControl::storeListLoaded()
{
	model->resetModel(store->loadAll());
}

AnimeInfo *MainControl::infoFromId(int id) const
{
	auto list = model->objects();
	auto res = std::find_if(list.constBegin(), list.constEnd(), [=](AnimeInfo *info){
		return info->id() == id;
	});
	if(res != list.constEnd())
		return *res;
	else
		return nullptr;
}

void MainControl::createAddControl(int id)
{
	auto control = new AddAnimeControl(this);
	control->setDeleteOnClose(true);
	connect(control, &AddAnimeControl::completed,
			this, &MainControl::internalAddInfo);
	control->setId(id);
	control->show();
}

void MainControl::internalAddInfo(AnimeInfo *info)
{
	if(store->contains(info->id()))
		CoreMessage::warning(tr("Anime duplicated"), tr("Anime \"%1\" is already in the list!").arg(info->title()));
	else {
		model->addObject(info);
		store->save(info);
		showStatus(tr("Added Anime: %1").arg(info->title()));
		coreApp->checkForSeasonUpdate(info);
	}
}
