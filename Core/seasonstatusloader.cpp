#include "seasonstatusloader.h"
using namespace QtRestClient;

SeasonStatusLoader::SeasonStatusLoader(QObject *parent) :
	QObject(parent),
	infoClass(new InfoClass(this)),
	updateQueue(),
	lastMax(0)
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
	if(updateQueue.isEmpty())
		emit completed();
	else {
		auto next = updateQueue.head();
		infoClass->getRelations(next->id())
				->onSucceeded([=](RestReply*, int, ProxerRelations *relation) {
			auto size = relation->data.size();
			if(size != next->lastKnownSeasons()) {
				next->setLastKnownSeasons(size);
				next->setHasNewSeasons(true);
				emit newSeasonsDetected(next);
			}
			relation->deleteLater();
			updateQueue.dequeue();
			checkNext();
		});
	}
}

void SeasonStatusLoader::error(const QString &errorString)
{
	updateQueue.clear();
	emit completed(errorString);
}
