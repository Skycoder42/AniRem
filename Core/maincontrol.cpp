#include "addanimecontrol.h"
#include "maincontrol.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>
#include <coremessage.h>
#include <QTimer>
#include <QDebug>

MainControl::MainControl(AnimeStore *store, QObject *parent) :
	Control(parent),
	store(store),
	model(new GenericListModel<AnimeInfo>(false, this)),
	_loading(false)
{
	connect(store, &AnimeStore::animeInfoListChanged,
			this, &MainControl::storeListLoaded);
}

GenericListModel<AnimeInfo> *MainControl::animeModel() const
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
	updateLoadStatus(true);

	//TODO perform loading
	Q_UNIMPLEMENTED();
}

void MainControl::uncheckAnime(int index)
{
	auto info = model->object(index);
	if(info) {
		info->setHasNewSeasons(false);
		store->saveAnime(info);
		//TODO add as settings: QDesktopServices::openUrl(info->relationsUrl());
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
	Q_UNIMPLEMENTED();
	//TODO "invent"
}

void MainControl::removeAnime(int index)
{
	auto info = model->object(index);
	if(info) {
		model->removeObject(index);
		showStatus(tr("Removed Anime: %1").arg(info->title()));
		store->forgetAnime(info->id());
	}
}

void MainControl::storeListLoaded(AnimeList list)
{
	model->resetModel(list);
}

void MainControl::onShow()
{
	updateLoadStatus(store->isLoading());
}

void MainControl::createAddControl(int id)
{
	auto control = new AddAnimeControl(this);
	connect(control, &AddAnimeControl::completed,
			this, &MainControl::internalAddInfo);
	control->setId(id);
	showControl(control);
}

void MainControl::internalAddInfo(AnimeInfo *info)
{
	if(store->containsAnime(info->id()))
		CoreMessage::warning(tr("Anime duplicated"), tr("Anime \"%1\" is already in the list!").arg(info->title()));
	else {
		model->addObject(info);
		store->saveAnime(info);
		showStatus(tr("Added Anime: %1").arg(info->title()));
	}
}
