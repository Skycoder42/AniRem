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

	Q_PROPERTY(AnimeList animeInfoList READ animeInfoList NOTIFY animeInfoListChanged)

public:
	explicit AnimeStore(QObject *parent = nullptr);
	~AnimeStore();

	AnimeList animeInfoList() const;
	bool containsAnime(int id) const;

public slots:
	void saveAnime(AnimeInfo *info);
	void saveAll(AnimeList infoList);
	void forgetAnime(int id);

signals:
	void loadingCompleted();

	void animeInfoListChanged(AnimeList infoList);

private slots:
	void loadAnimes();
	void setInternal(AnimeList infoList, bool emitComplete);

	void saveQuitApp();

	void showError(const QString &error);

private:
	static const QString dbName;

	QThreadPool *tp;
	CountLock lock;

	//the store owns all anime info objects!
	QHash<int, AnimeInfo*> infoMap;
};

#endif // ANIMESTORE_H
