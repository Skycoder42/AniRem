#include "seasonstatusloader.h"

#include "proxerapi.h"
#include "apihelper.h"

using namespace QtRestClient;

SeasonStatusLoader::SeasonStatusLoader(QObject *parent) :
	QObject(parent),
	_store(new AniremStore(this)),
	_infoClass(ProxerApi::factory().info().instance(this)),
	_settings(nullptr),
	_updateQueue(),
	_lastMax(0),
	_anyUpdated(false)
{
	connect(_infoClass, &InfoClass::apiError,
			this, &SeasonStatusLoader::error);
}

bool SeasonStatusLoader::isUpdating() const
{
	return !_updateQueue.isEmpty();
}

void SeasonStatusLoader::preClean()
{
	delete _store;
	_store = nullptr;
}

void SeasonStatusLoader::checkForUpdates(bool useInterval)
{
	auto updateList = _store->loadAll();
	if(updateList.isEmpty())
		return;

	//sort by last update check time
	std::sort(updateList.begin(), updateList.end(), [](const AnimeInfo &left, const AnimeInfo &right){
		return left.lastUpdateCheck() < right.lastUpdateCheck();
	});

	//reduce to allowed list size
	int maxSize = _settings->updates.checkLimit;
	if(maxSize > 0)
		updateList = updateList.mid(0, maxSize);

	//reduce to allowed interval
	if(useInterval) {
		auto interval = _settings->updates.autoCheck.get().toInt();
		if(interval == 0) {
			emit completed(false);
			return;
		}

		for(auto i = 0; i < updateList.size(); i++) {
			if(updateList[i].lastUpdateCheck().date().daysTo(QDate::currentDate()) < interval) {
				updateList = updateList.mid(0, i);
				break;
			}
		}
	}

	if(updateList.isEmpty())
		emit completed(false);
	else
		addInfos(updateList);
}

void SeasonStatusLoader::checkForUpdates(const AnimeInfo &info)
{
	addInfos({info});
}

void SeasonStatusLoader::checkNext()
{
	emit updated(_lastMax - _updateQueue.size(), _lastMax);
	if(_updateQueue.isEmpty()) {
		emit completed(_anyUpdated);
		_lastMax = 0;
		_anyUpdated = false;
	} else {
		auto next = _updateQueue.head();
		auto rep = _infoClass->getRelations(next.id(), _settings->updates.hentai);
		rep->onSucceeded([this, next](int code, ProxerRelations relation) {
			auto animeInfo = next;
			if(!ApiHelper::testValid(code, relation)) {
				error(relation.message(), code, RestReply::FailureError);
				return;
			}

			QHash<AnimeInfo::SeasonType, int> state;
			for(const auto &season : relation.data()) {
				auto type = toType(season.medium());
				state[type]++;
			}

			auto infoState = animeInfo.seasonState();
			auto wasEmpty = infoState.isEmpty();
			for(auto it = state.constBegin(); it != state.constEnd(); it++) {
				if(infoState[it.key()].first != it.value()) {
					infoState[it.key()].first = it.value();
					infoState[it.key()].second = !wasEmpty;
				}
			}
			animeInfo.setSeasonState(infoState);
			animeInfo.setLastUpdateCheck(QDateTime::currentDateTime());

			try {
				_store->save(animeInfo);

				if(animeInfo.hasNewSeasons())
					_anyUpdated = true;

				_updateQueue.dequeue();
				checkNext();
			} catch(QException &e) {
				qCritical() << "Failed to save info of id" << animeInfo.id()
							<< "with error:" << e.what();
				_updateQueue.clear();
				emit completed(_anyUpdated, tr("Failed to store update anime infos"));
				_anyUpdated = false;
			}
		});
	}
}

void SeasonStatusLoader::error(const QString &errorString, int errorCode, RestReply::ErrorType errorType)
{
	_updateQueue.clear();
	emit completed(_anyUpdated, ApiHelper::formatError(errorString, errorCode, errorType));
	_anyUpdated = false;
}

AnimeInfo::SeasonType SeasonStatusLoader::toType(const QString &medium)
{
	if(medium == QStringLiteral("animeseries"))
		return AnimeInfo::Anime;
	else if(medium == QStringLiteral("movie"))
		return AnimeInfo::Movie;
	else if(medium == QStringLiteral("ova"))
		return AnimeInfo::Ova;
	else if(medium == QStringLiteral("hentai"))
		return AnimeInfo::Hentai;
	else if(medium == QStringLiteral("mangaseries"))
		return AnimeInfo::Manga;
	else if(medium == QStringLiteral("oneshot"))
		return AnimeInfo::Oneshot;
	else if(medium == QStringLiteral("doujin"))
		return AnimeInfo::Doujin;
	else if(medium == QStringLiteral("hmanga"))
		return AnimeInfo::Hmanga;
	else {
		qWarning() << "Unknown medium:" << medium;
		return AnimeInfo::Unknown;
	}
}

void SeasonStatusLoader::addInfos(const QList<AnimeInfo> &infos)
{
	auto empty = _updateQueue.isEmpty();
	_updateQueue.append(infos);
	_lastMax += infos.size();
	if(empty) {
		emit started();
		checkNext();
	}
}
