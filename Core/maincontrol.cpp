#include "maincontrol.h"

MainControl::MainControl(AnimeStore *store, QObject *parent) :
	Control(parent),
	store(store),
	model(new GenericListModel<AnimeInfo>(false, this))
{
	connect(store, &AnimeStore::animeInfoListChanged,
			this, &MainControl::setAnimeList);
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

void MainControl::addAnime()
{
	Q_UNIMPLEMENTED();
}

void MainControl::addAnimeFromClipboard()
{
	Q_UNIMPLEMENTED();
}

void MainControl::showDetails(int id)
{
	Q_UNIMPLEMENTED();
}

void MainControl::setAnimeList(AnimeList list)
{

}

void MainControl::onShow()
{
	emit updateLoadStatus(false);
}
