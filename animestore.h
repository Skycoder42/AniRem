#ifndef ANIMESTORE_H
#define ANIMESTORE_H

#include <QObject>
#include <QJsonArray>
#include <QReadWriteLock>
#include <QThreadPool>
#include "animeinfo.h"

class AnimeStore : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QList<AnimeInfo> animeInfoList READ animeInfoList NOTIFY animeInfoListChanged)

public:
	explicit AnimeStore(QObject *parent = nullptr);

	QList<AnimeInfo> animeInfoList() const;

public slots:
	void saveAnime(AnimeInfo info);
	void saveAll(QList<AnimeInfo> infoList);
	bool forgetAnime(int id);

signals:
	void loadingCompleted();
	void storeError(QString error);

	void animeInfoListChanged(QList<AnimeInfo> infoList);

private slots:
	void loadAnimes();
	void setInternal(QList<AnimeInfo> infoList);

	void saveQuitApp();

private:
	QThreadPool *tp;
	QReadWriteLock saveLock;
	bool canSave;

	QList<AnimeInfo> infoList;

	QJsonArray loadList();
	void saveList(const QJsonArray &array);
	QString imgPath(int id);
};

#endif // ANIMESTORE_H
