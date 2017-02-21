#include "seasonstatusloader.h"
using namespace QtRestClient;

SeasonStatusLoader::SeasonStatusLoader(QObject *parent) :
	QObject(parent),
	infoClass(new InfoClass(this)),
	updateQueue(),
	lastMax(0),
	progress(0),
	anyUpdated(false)
{
	connect(infoClass, &InfoClass::apiError,
			this, &SeasonStatusLoader::error);
}

void SeasonStatusLoader::checkForUpdates(const QList<AnimeInfo *> &animeList, bool forceHasUpdates)
{
	if(forceHasUpdates)
		anyUpdated = true;
	auto empty = updateQueue.isEmpty();
	updateQueue.append(animeList);
	lastMax += animeList.size();
	if(empty)
		checkNext();
}

void SeasonStatusLoader::checkNext()
{
	emit updated(lastMax - updateQueue.size(), lastMax);
	if(updateQueue.isEmpty()) {
		emit completed(anyUpdated);
		anyUpdated = false;
	} else {
		auto next = updateQueue.head();
		infoClass->getRelations(next->id())
				->onSucceeded([=](RestReply*, int code, ProxerRelations *relation) {
			if(!infoClass->testValid(code, relation)) {
				relation->deleteLater();
				return;
			}

			QHash<AnimeInfo::SeasonType, int> state;
			foreach(auto season, relation->data) {
				auto type = toType(season->medium);
				state[type]++;
			}

			auto infoState = next->seasonState();
			auto wasEmpty = infoState.isEmpty();
			for(auto it = state.constBegin(); it != state.constEnd(); it++) {
				if(infoState[it.key()].first != it.value()) {
					infoState[it.key()].first = it.value();
					infoState[it.key()].second = !wasEmpty;
				}
			}
			next->setSeasonState(infoState);

			next->setLastUpdateCheck(QDate::currentDate());
			emit animeInfoUpdated(next);

			if(next->hasNewSeasons())
				anyUpdated = true;

			relation->deleteLater();
			updateQueue.dequeue();
			checkNext();
		});
	}
}

void SeasonStatusLoader::error(const QString &errorString)
{
	updateQueue.clear();
	emit completed(anyUpdated, errorString);
	anyUpdated = false;
}

AnimeInfo::SeasonType SeasonStatusLoader::toType(const QString &medium)
{
	if(medium == "animeseries")
		return AnimeInfo::Anime;
	else if(medium == "movie")
		return AnimeInfo::Movie;
	else if(medium == "ova")
		return AnimeInfo::Ova;
	else if(medium == "hentai")
		return AnimeInfo::Hentai;
	else if(medium == "mangaseries")
		return AnimeInfo::Manga;
	else if(medium == "oneshot")
		return AnimeInfo::Oneshot;
	else if(medium == "doujin")
		return AnimeInfo::Doujin;
	else if(medium == "hmanga")
		return AnimeInfo::Hmanga;
	else
		return AnimeInfo::Unknown;
}
