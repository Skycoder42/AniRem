#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include <QString>
#include <QPixmap>
#include <QUrl>
#include <QSharedDataPointer>

struct AnimeInfoData;
class AnimeInfo//TODO shared data
{
public:
	AnimeInfo();
	AnimeInfo(int id, const QString &title);
	AnimeInfo(const AnimeInfo &other);
	~AnimeInfo();

	int id() const;
	QString title() const;
	QPixmap previewImage() const;
	void setPreviewImage(QPixmap previewImage);

	int lastKnownSeasons() const;
	void setLastKnownSeasons(int lastKnownSeasons);
	bool hasNewSeasons() const;
	void setHasNewSeasons(bool hasNewSeasons);

	QUrl relationsUrl() const;

	AnimeInfo &operator =(const AnimeInfo &other);

private:
	QSharedDataPointer<AnimeInfoData> data;
};

Q_DECLARE_METATYPE(AnimeInfo)

#endif // ANIMEINFO_H
