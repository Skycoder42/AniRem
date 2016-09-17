#include "seasonstatusloader.h"

SeasonStatusLoader::SeasonStatusLoader(AnimeStore *store, QObject *parent) :
	QObject(parent),
	store(store),
	connector(new ProxerConnector(this)),
	animes(),
	cIndex(0),
	hasNew(false)
{
	connect(this->connector, &ProxerConnector::seasonsLoaded,
			this, &SeasonStatusLoader::seasonsLoaded);
}

void SeasonStatusLoader::load()
{
	this->animes = this->store->animeInfoList();
	this->cIndex = 0;
	this->hasNew = false;
	if(this->animes.isEmpty())
		emit completed(false);
	else
		this->connector->loadSeasonCount(this->animes[0].id);
}

void SeasonStatusLoader::seasonsLoaded(int id, int count)
{
	auto &info = this->animes[this->cIndex];
	Q_ASSERT(id == info.id);

	if(info.lastKnownSeasons != count) {
		info.hasNewSeasons = info.lastKnownSeasons != -1;
		info.lastKnownSeasons = count;
	}
	if(info.hasNewSeasons)
		this->hasNew = true;

	emit updated(++this->cIndex, this->animes.size());
	if(this->cIndex == this->animes.size()) {
		this->store->saveAll(this->animes);
		this->animes.clear();
		emit completed(this->hasNew);
	} else
		this->connector->loadSeasonCount(this->animes[this->cIndex].id);
}
