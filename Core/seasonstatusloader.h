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
	void checkForUpdates(const AnimeList &animeList, bool forceHasUpdates = false);

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

	static AnimeInfo::SeasonType toType(const QString &medium);
};

#endif // SEASONSTATUSLOADER_H
