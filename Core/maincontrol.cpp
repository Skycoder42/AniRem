#include "maincontrol.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>
#include <coremessage.h>

MainControl::MainControl(AnimeStore *store, QObject *parent) :
	Control(parent),
	store(store),
	model(new GenericListModel<AnimeInfo>(false, this))
{
	connect(store, &AnimeStore::animeInfoListChanged,
			this, &MainControl::storeListLoaded);
}

GenericListModel<AnimeInfo> *MainControl::animeModel() const
{
	return model;
}

void MainControl::reload()
{
	emit updateLoadStatus(false);
	emit showStatus(tr("Checking for new seasons…"), true);

	//TODO perform loading
	Q_UNIMPLEMENTED();
}

void MainControl::uncheckAnime(const QModelIndex index)
{
	auto info = model->object(index);
	if(info) {
		info->setHasNewSeasons(false);
		store->saveAnime(info);
		QDesktopServices::openUrl(info->relationsUrl());
	}
}

void MainControl::addAnime()
{
	Q_UNIMPLEMENTED();
	//TODO show anime control, then add info to store
	//internalSave(...);
	//showStatus(tr("Added Anime: %1").arg(info->title()));
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

	if(id != -1) {
		Q_UNIMPLEMENTED();
		//TODO show anime control, then add info to store
		//internalSave(...);
		//showStatus(tr("Added Anime: %1").arg(info->title()));
	} else
		showStatus(tr("Clipboard does not contain a proxer url or an id"));
}

void MainControl::showDetails(int id)
{
	Q_UNIMPLEMENTED();
	//TODO "invent"
}

void MainControl::removeAnime(QModelIndex index)
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
	emit updateLoadStatus(false);
}

void MainControl::internalSave(AnimeInfo *info)
{
	if(store->containsAnime(info->id()))
		CoreMessage::warning(tr("Anime duplicated"), tr("Anime \"%1\" is already in the list!").arg(info->title()));
	else {
		model->addObject(info);
		store->saveAnime(info);
	}
}
