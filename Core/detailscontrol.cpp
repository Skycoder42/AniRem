#include "detailscontrol.h"

DetailsControl::DetailsControl(QObject *parent) :
	Control(parent),
	_animeInfo(nullptr)
{}

AnimeInfo *DetailsControl::animeInfo() const
{
	return _animeInfo;
}

void DetailsControl::setAnimeInfo(AnimeInfo *animeInfo)
{
	if (_animeInfo == animeInfo)
		return;

	_animeInfo = animeInfo;
	emit animeInfoChanged(animeInfo);
}
