#ifndef ANIMESTORE_H
#define ANIMESTORE_H

#include <QObject>
#include <QJsonArray>
#include <QReadWriteLock>
#include "animeinfo.h"

class AnimeStore : public QObject
{
	Q_OBJECT
public:
	explicit AnimeStore(QObject *parent = nullptr);
	~AnimeStore();

public slots:
	void loadAnimes();
	void saveAnime(const AnimeInfo &info);

signals:
	void loadingCompleted(QList<AnimeInfo> infoList);
	void storeError(QString error);

private:
	QReadWriteLock saveLock;
	bool canSave;

	QJsonArray loadList();
	void saveList(const QJsonArray &array);
};

#endif // ANIMESTORE_H
