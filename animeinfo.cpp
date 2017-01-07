#include "animeinfo.h"

struct AnimeInfoData : public QSharedData
{
	AnimeInfoData(int id = -1, const QString &title = {});
	AnimeInfoData(const AnimeInfoData &other);

	int id;
	QString title;
	QPixmap previewImage;

	int lastKnownSeasons;
	bool hasNewSeasons;
};

AnimeInfo::AnimeInfo() :
	data(new AnimeInfoData())
{}

AnimeInfo::AnimeInfo(int id, const QString &title) :
	data(new AnimeInfoData(id, title))
{}

AnimeInfo::AnimeInfo(const AnimeInfo &other) :
	data(other.data)
{}

AnimeInfo::~AnimeInfo() {}

int AnimeInfo::id() const
{
	return data->id;
}

QString AnimeInfo::title() const
{
	return data->title;
}

QPixmap AnimeInfo::previewImage() const
{
	return data->previewImage;
}

void AnimeInfo::setPreviewImage(QPixmap previewImage)
{
	data->previewImage = previewImage;
}

int AnimeInfo::lastKnownSeasons() const
{
	return data->lastKnownSeasons;
}

void AnimeInfo::setLastKnownSeasons(int lastKnownSeasons)
{
	data->lastKnownSeasons = lastKnownSeasons;
}

bool AnimeInfo::hasNewSeasons() const
{
	return data->hasNewSeasons;
}

void AnimeInfo::setHasNewSeasons(bool hasNewSeasons)
{
	data->hasNewSeasons = hasNewSeasons;
}

QUrl AnimeInfo::relationsUrl() const
{
	return QStringLiteral("https://proxer.me/info/%1/relation").arg(data->id);
}

AnimeInfo &AnimeInfo::operator =(const AnimeInfo &other)
{
	data = other.data;
	return *this;
}

AnimeInfoData::AnimeInfoData(int id, const QString &title) :
	QSharedData(),
	id(id),
	title(title),
	previewImage(),
	lastKnownSeasons(-1),
	hasNewSeasons(false)
{}

AnimeInfoData::AnimeInfoData(const AnimeInfoData &other) :
	QSharedData(),
	id(other.id),
	title(other.title),
	previewImage(other.previewImage),
	lastKnownSeasons(other.lastKnownSeasons),
	hasNewSeasons(other.hasNewSeasons)
{}
