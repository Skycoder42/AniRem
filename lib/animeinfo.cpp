#include <utility>

#include "animeinfo.h"
#include <QMutex>

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

	mutable int seasonCount = -1; //TODO make threadsafe?
	mutable int hasNewSeasons = -1;
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
		d->seasonCount = 0;
		for(auto info : d->seasonState)
			d->seasonCount += info.first;
	}

	return d->seasonCount;
}

bool AnimeInfo::hasNewSeasons() const
{
	if(d->hasNewSeasons < 0) {
		d->hasNewSeasons = false;
		for(auto info : d->seasonState) {
			if(info.second) {
				d->hasNewSeasons = true;
				break;
			}
		}
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

void AnimeInfo::setId(int id)
{
	d->id = id;
}

void AnimeInfo::setTitle(const QString &title)
{
	d->title = title;
}
