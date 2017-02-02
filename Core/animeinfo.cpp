#include "animeinfo.h"

AnimeInfo::AnimeInfo(int id, const QString &title, QObject *parent) :
	QObject(parent),
	_id(id),
	_title(title),
	_lastKnownSeasons(-1),
	_hasNewSeasons(false)
{
	setObjectName(QStringLiteral("AnimeInfo<%1>").arg(id));
}

int AnimeInfo::id() const
{
	return _id;
}

QString AnimeInfo::title() const
{
	return _title;
}

int AnimeInfo::lastKnownSeasons() const
{
	return _lastKnownSeasons;
}

bool AnimeInfo::hasNewSeasons() const
{
	return _hasNewSeasons;
}

QDate AnimeInfo::lastUpdateCheck() const
{
	return _lastUpdateCheck;
}

QUrl AnimeInfo::relationsUrl() const
{
	return QStringLiteral("https://proxer.me/info/%1/relation").arg(_id);
}

void AnimeInfo::setLastKnownSeasons(int lastKnownSeasons)
{
	if(_lastKnownSeasons != lastKnownSeasons) {
		_lastKnownSeasons = lastKnownSeasons;
		emit lastKnownSeasonsChanged(lastKnownSeasons);
	}
}

void AnimeInfo::setHasNewSeasons(bool hasNewSeasons)
{
	if(_hasNewSeasons != hasNewSeasons) {
		_hasNewSeasons = hasNewSeasons;
		emit hasNewSeasonsChanged(hasNewSeasons);
	}
}

void AnimeInfo::setLastUpdateCheck(QDate lastUpdateCheck)
{
	if (_lastUpdateCheck == lastUpdateCheck)
		return;

	_lastUpdateCheck = lastUpdateCheck;
	emit lastUpdateCheckChanged(lastUpdateCheck);
}
