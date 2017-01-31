#ifndef SEASONSTATUSLOADER_H
#define SEASONSTATUSLOADER_H

#include <QObject>
#include <QQueue>
#include <ProxerApi/infoclass.h>
#include "animeinfo.h"

class SeasonStatusLoader : public QObject
{
	Q_OBJECT
public:
	explicit SeasonStatusLoader(QObject *parent = nullptr);

public slots:
	void checkForUpdates(const AnimeList &animeList);

signals:
	void updated(int value, int max);
	void animeInfoUpdated(AnimeInfo *anime);
	void completed(bool hasUpdates, const QString &errorString = {});

private slots:
	void checkNext();
	void error(const QString &errorString);

private:
	InfoClass *infoClass;
	QQueue<AnimeInfo*> updateQueue;
	int lastMax;
	int progress;
	bool anyUpdated;
};

#endif // SEASONSTATUSLOADER_H
