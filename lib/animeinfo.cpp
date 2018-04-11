#include <utility>
#include <QDebug>

#include "animeinfo.h"

class AnimeInfoData : public QSharedData
{
public:
	AnimeInfoData(int pId = -1, QString pTitle = {}) :
		id(pId),
		title(std::move(pTitle))
	{}
	AnimeInfoData(const AnimeInfoData &other) = default;

	int id;
	QString title;

	AnimeInfo::SeasonMap seasonState;
	QDateTime lastUpdateCheck;

	mutable QAtomicInt seasonCount = -1;
	mutable QAtomicInt hasNewSeasons = -1;

	bool operator==(const AnimeInfoData &other) const {
		return id == other.id &&
				title == other.title &&
				seasonState == other.seasonState &&
				lastUpdateCheck == other.lastUpdateCheck;
	}
	bool operator!=(const AnimeInfoData &other) const {
		return id != other.id ||
				title != other.title ||
				seasonState != other.seasonState ||
				lastUpdateCheck != other.lastUpdateCheck;
	}
};



AnimeInfo::AnimeInfo() :
	d(new AnimeInfoData())
{}

AnimeInfo::AnimeInfo(int id, const QString &title) :
	d(new AnimeInfoData(id, title))
{}

AnimeInfo::operator bool() const
{
	return d && d->id >= 0;
}

bool AnimeInfo::operator!() const
{
	return !d || d->id < 0;
}

AnimeInfo::AnimeInfo(const AnimeInfo &other) = default;

AnimeInfo &AnimeInfo::operator=(const AnimeInfo &other) = default;

AnimeInfo::~AnimeInfo() = default;

int AnimeInfo::id() const
{
	return d->id;
}

QString AnimeInfo::title() const
{
	return d->title;
}

AnimeInfo::SeasonMap AnimeInfo::seasonState() const
{
	return d->seasonState;
}

AnimeInfo::SeasonInfo AnimeInfo::seasonInfo(AnimeInfo::SeasonType type) const
{
	return d->seasonState.value(type, {0, false});
}

int AnimeInfo::totalSeasonCount() const
{
	if(d->seasonCount < 0) {
		auto cnt = 0;
		for(auto info : d->seasonState)
			cnt += info.first;
		d->seasonCount = cnt;
	}

	return d->seasonCount;
}

bool AnimeInfo::hasNewSeasons() const
{
	if(d->hasNewSeasons < 0) {
		auto cnt = false;
		for(auto info : d->seasonState) {
			if(info.second) {
				cnt = true;
				break;
			}
		}
		d->hasNewSeasons = cnt;
	}

	return d->hasNewSeasons;
}

QDateTime AnimeInfo::lastUpdateCheck() const
{
	return d->lastUpdateCheck;
}

QUrl AnimeInfo::relationsUrl() const
{
	return QStringLiteral("https://proxer.me/info/%1/relation").arg(d->id);
}

void AnimeInfo::setAllUnchanged()
{
	auto changed = false;
	for(auto it = d->seasonState.begin(); it != d->seasonState.end(); it++) {
		if(it.value().second) {
			it.value().second = false;
			changed = true;
		}
	}

	if(changed)
		d->hasNewSeasons = -1;
}

void AnimeInfo::setSeasonState(const SeasonMap &seasonState)
{
	if (d->seasonState == seasonState)
		return;

	d->seasonState = seasonState;
	d->seasonCount = -1;
	d->hasNewSeasons = -1;
}

void AnimeInfo::setLastUpdateCheck(QDateTime lastUpdateCheck)
{
	d->lastUpdateCheck = lastUpdateCheck;
}

void AnimeInfo::setSeasonInfo(AnimeInfo::SeasonType type, AnimeInfo::SeasonInfo info)
{
	if (d->seasonState[type] == info)
		return;

	d->seasonState[type] = info;
	d->seasonCount = -1;
	d->hasNewSeasons = -1;
}

QString AnimeInfo::typeToString(AnimeInfo::SeasonType type)
{
	switch (type) {
	case AnimeInfo::Anime:
		return tr("Animes");
	case AnimeInfo::Movie:
		return tr("Movies");
	case AnimeInfo::Ova:
		return tr("OVAs");
	case AnimeInfo::Hentai:
		return tr("Hentais");
	case AnimeInfo::Manga:
		return tr("Mangas");
	case AnimeInfo::Oneshot:
		return tr("Oneshots");
	case AnimeInfo::Doujin:
		return tr("Doujinshi");
	case AnimeInfo::Hmanga:
		return tr("H-Mangas");
	case AnimeInfo::Unknown:
		return tr("<i>Unknown</i>");
	default:
		Q_UNREACHABLE();
		return {};
	}
}

bool AnimeInfo::operator==(const AnimeInfo &other) const
{
	return (*d) == *(other.d);
}

bool AnimeInfo::operator!=(const AnimeInfo &other) const
{
	return d != other.d && (*d) != *(other.d);
}

std::tuple<AnimeInfo::SeasonType, bool> AnimeInfo::apiMediumToType(const QString &medium, SyncedSettings *settings)
{
	if(medium == QStringLiteral("animeseries"))
		return std::make_tuple(AnimeInfo::Anime,  settings->content.type.anime.get());
	else if(medium == QStringLiteral("movie"))
		return std::make_tuple(AnimeInfo::Movie, settings->content.type.movie.get());
	else if(medium == QStringLiteral("ova"))
		return std::make_tuple(AnimeInfo::Ova, settings->content.type.ova.get());
	else if(medium == QStringLiteral("hentai"))
		return std::make_tuple(AnimeInfo::Hentai, settings->content.type.hentai.get());
	else if(medium == QStringLiteral("mangaseries"))
		return std::make_tuple(AnimeInfo::Manga, settings->content.type.manga.get());
	else if(medium == QStringLiteral("oneshot"))
		return std::make_tuple(AnimeInfo::Oneshot, settings->content.type.oneshot.get());
	else if(medium == QStringLiteral("doujin"))
		return std::make_tuple(AnimeInfo::Doujin, settings->content.type.doujin.get());
	else if(medium == QStringLiteral("hmanga"))
		return std::make_tuple(AnimeInfo::Hmanga, settings->content.type.hmanga.get());
	else {
		qWarning() << "Unknown medium:" << medium;
		return std::make_tuple(AnimeInfo::Unknown, true);
	}
}

void AnimeInfo::setId(int id)
{
	d->id = id;
}

void AnimeInfo::setTitle(const QString &title)
{
	d->title = title;
}
