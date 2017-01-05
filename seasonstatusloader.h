#ifndef SEASONSTATUSLOADER_H
#define SEASONSTATUSLOADER_H

#include <QObject>
#include "animeinfo.h"
#include "proxerconnector.h"
#include "animestore.h"

class SeasonStatusLoader : public QObject
{
	Q_OBJECT
public:
	explicit SeasonStatusLoader(AnimeStore *store, QObject *parent = nullptr);

public slots:
	void load();

signals:
	void updated(int value, int max);
	void completed(bool hasNewSeasons);
	void errorMessage(QString title, QString message);

private slots:
	void seasonsLoaded(int id, int count);

private:
	AnimeStore *store;
	ProxerConnector *connector;

	QList<AnimeInfo> animes;
	int cIndex;
	bool hasNew;
};

#endif // SEASONSTATUSLOADER_H
