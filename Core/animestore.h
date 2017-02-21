#ifndef ANIMESTORE_H
#define ANIMESTORE_H

#include "core_global.h"
#include "animeinfo.h"
#include "countlock.h"
#include <QObject>
#include <QSemaphore>
#include <QThreadPool>
#include <cachingdatastore.h>

class CORESHARED_EXPORT AnimeStore : public QObject
{
	Q_OBJECT

	Q_PROPERTY(AnimeList animeInfoList READ animeInfoList NOTIFY animeInfoListChanged)

public:
	explicit AnimeStore(QObject *parent = nullptr);

	AnimeList animeInfoList() const;
	AnimeInfo *animeInfo(int id) const;
	bool containsAnime(int id) const;

public slots:
	void saveAnime(AnimeInfo *info);
	void forgetAnime(int id);

signals:
	void storeLoaded();
	void animeInfoListChanged(AnimeList infoList);

private slots:
	void showError(const QString &error);

private:
	QtDataSync::CachingDataStore<AnimeInfo*> *store;
};

#endif // ANIMESTORE_H
