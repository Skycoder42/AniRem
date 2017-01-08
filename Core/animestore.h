#ifndef ANIMESTORE_H
#define ANIMESTORE_H

#include "core_global.h"
#include "animeinfo.h"
#include "countlock.h"
#include <QObject>
#include <QSemaphore>
#include <QThreadPool>

class CORESHARED_EXPORT AnimeStore : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QList<AnimePtr> animeInfoList READ animeInfoList NOTIFY animeInfoListChanged)

public:
	explicit AnimeStore(QObject *parent = nullptr);
	~AnimeStore();

	AnimeList animeInfoList() const;

public slots:
	void saveAnime(AnimePtr info);
	void saveAll(AnimeList infoList);
	void forgetAnime(int id);

signals:
	void loadingCompleted();
	void storeError(QString error);

	void animeInfoListChanged(AnimeList infoList);

private slots:
	void loadAnimes();
	void setInternal(AnimeList infoList, bool emitComplete);

	void saveQuitApp();

private:
	static const QString dbName;

	QThreadPool *tp;
	CountLock lock;

	QHash<int, AnimePtr> infoMap;
};

#endif // ANIMESTORE_H
