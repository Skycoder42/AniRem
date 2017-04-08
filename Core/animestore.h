#ifndef ANIMESTORE_H
#define ANIMESTORE_H

#include "animeinfo.h"
#include <QObject>
#include <QSemaphore>
#include <QThreadPool>
#include <cachingdatastore.h>

class AnimeStore : public QtDataSync::CachingDataStore<AnimeInfo*, int>
{
	Q_OBJECT

public:
	explicit AnimeStore(QObject *parent = nullptr);

signals:
	void animeInfoListChanged(QList<AnimeInfo*> infoList);
};

#endif // ANIMESTORE_H
