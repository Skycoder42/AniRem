#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include <QString>
#include <QPixmap>

struct AnimeInfo
{
	int id;
	QString title;
	QPixmap previewImage;

	int lastKnownSeasons;
	bool hasNewSeasons;

	AnimeInfo();
};

#endif // ANIMEINFO_H
