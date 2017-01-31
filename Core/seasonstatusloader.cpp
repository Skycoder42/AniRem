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

void SeasonStatusLoader::checkForUpdates(const AnimeList &animeList)
{
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
				->onSucceeded([=](RestReply*, int, ProxerRelations *relation) {
			auto size = relation->data.size();
			if(size != next->lastKnownSeasons()) {
				next->setHasNewSeasons(next->lastKnownSeasons() != -1);//DEBUG set to true to test easily
				next->setLastKnownSeasons(size);
			}
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
