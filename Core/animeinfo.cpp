#include "animeinfo.h"

AnimeInfo::AnimeInfo(int id, const QString &title, QObject *parent) :
	QObject(parent),
	_id(id),
	_title(title),
	_seasonState(),
	_seasonCount(-1),
	_hasNewSeasons(-1)
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

QHash<AnimeInfo::SeasonType, AnimeInfo::SeasonInfo> AnimeInfo::seasonState() const
{
	return _seasonState;
}

AnimeInfo::SeasonInfo AnimeInfo::seasonInfo(AnimeInfo::SeasonType type) const
{
	return _seasonState.value(type, {0, false});
}

int AnimeInfo::totalSeasonCount() const
{
	if(_seasonCount < 0) {
		_seasonCount = 0;
		foreach (auto info, _seasonState)
			_seasonCount += info.first;
	}

	return _seasonCount;
}

bool AnimeInfo::hasNewSeasons() const
{
	if(_hasNewSeasons < 0) {
		_hasNewSeasons = false;
		foreach (auto info, _seasonState) {
			if(info.second) {
				_hasNewSeasons = true;
				break;
			}
		}
	}

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

void AnimeInfo::setSeasonState(QHash<SeasonType, SeasonInfo> seasonState)
{
	if (_seasonState == seasonState)
		return;

	_seasonState = seasonState;
	_seasonCount = -1;
	_hasNewSeasons = -1;
	emit seasonStateChanged();
}

void AnimeInfo::setSeasonInfo(AnimeInfo::SeasonType type, AnimeInfo::SeasonInfo info)
{
	if (_seasonState[type] == info)
		return;

	_seasonState[type] = info;
	_seasonCount = -1;
	_hasNewSeasons = -1;
	emit seasonStateChanged();
}

void AnimeInfo::setAllUnchanged()
{
	auto changed = false;
	for(auto it = _seasonState.begin(); it != _seasonState.end(); it++) {
		if(it.value().second) {
			it.value().second = false;
			changed = true;
		}
	}

	if(changed) {
		_hasNewSeasons = -1;
		emit seasonStateChanged();
	}
}

void AnimeInfo::setLastUpdateCheck(QDate lastUpdateCheck)
{
	if (_lastUpdateCheck == lastUpdateCheck)
		return;

	_lastUpdateCheck = lastUpdateCheck;
	emit lastUpdateCheckChanged(lastUpdateCheck);
}
