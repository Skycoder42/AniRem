#include "animeinfo.h"

QUrl AnimeInfo::relationsUrl() const
{
	return QStringLiteral("https://proxer.me/info/%1/relation").arg(this->id);
}

AnimeInfo::AnimeInfo() :
	id(-1),
	title(),
	previewImage(),
	lastKnownSeasons(-1),
	hasNewSeasons(false)
{}
