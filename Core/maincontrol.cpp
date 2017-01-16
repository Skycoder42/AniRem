#include "maincontrol.h"
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
}

void MainControl::uncheckAnime(const QModelIndex index)
{
	auto info = model->object(index);
	if(info) {
		info->setHasNewSeasons(false);
		store->saveAnime(info);
	}
}

void MainControl::addAnime()
{
	Q_UNIMPLEMENTED();
	//TODO show anime control, then add info to store
	//internalSave(...);
}

void MainControl::addAnimeFromClipboard()
{
	Q_UNIMPLEMENTED();
	//TODO show anime control, then add info to store
	//internalSave(...);
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
