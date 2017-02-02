#include "animeinfo.h"

AnimeInfo::AnimeInfo(int id, const QString &title, QObject *parent) :
	QObject(parent),
	_id(id),
	_title(title),
	_seasonState(),
	_hasNewSeasons(false),
	_seasonCount(-1)
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

QHash<AnimeInfo::SeasonType, int> AnimeInfo::seasonState() const
{
	return _seasonState;
}

int AnimeInfo::seasonCount(AnimeInfo::SeasonType type) const
{
	return _seasonState.value(type, 0);
}

int AnimeInfo::totalSeasonCount() const
{
	if(_seasonCount < 0) {
		_seasonCount = 0;
		foreach (auto count, _seasonState)
			_seasonCount += count;
	}

	return _seasonCount;
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

void AnimeInfo::setSeasonState(QHash<AnimeInfo::SeasonType, int> seasonState)
{
	if (_seasonState == seasonState)
		return;

	_seasonState = seasonState;
	_seasonCount = -1;
	emit seasonStateChanged(seasonState);
	emit totalSeasonCountChanged();
}

void AnimeInfo::setSeasonCount(AnimeInfo::SeasonType type, int count)
{
	_seasonState.insert(type, count);
	_seasonCount = -1;
	emit seasonStateChanged(_seasonState);
	emit totalSeasonCountChanged();
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
