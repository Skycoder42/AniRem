#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include "core_global.h"
#include <QDate>
#include <QObject>
#include <QPixmap>
#include <QSharedPointer>
#include <QUrl>

class CORESHARED_EXPORT AnimeInfo : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id CONSTANT)
	Q_PROPERTY(QString title READ title CONSTANT)
	Q_PROPERTY(QHash<SeasonType, int> seasonState READ seasonState WRITE setSeasonState NOTIFY seasonStateChanged)
	Q_PROPERTY(int totalSeasonCount READ totalSeasonCount NOTIFY totalSeasonCountChanged STORED false)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons WRITE setHasNewSeasons NOTIFY hasNewSeasonsChanged)
	Q_PROPERTY(QDate lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck NOTIFY lastUpdateCheckChanged)
	Q_PROPERTY(QUrl relationsUrl READ relationsUrl CONSTANT STORED false)

public:
	enum SeasonType {
		Anime,
		Movie,
		Ova,
		Hentai,
		Manga,
		Oneshot,
		Doujin,
		Hmanga,
		Unknown
	};
	Q_ENUM(SeasonType)

	explicit AnimeInfo(int id, const QString &title, QObject *parent = nullptr);

	int id() const;
	QString title() const;
	QHash<SeasonType, int> seasonState() const;
	int seasonCount(SeasonType type) const;
	int totalSeasonCount() const;
	bool hasNewSeasons() const;
	QDate lastUpdateCheck() const;

	QUrl relationsUrl() const;

public slots:
	void setSeasonState(QHash<SeasonType, int> seasonState);
	void setSeasonCount(SeasonType type, int count);
	void setHasNewSeasons(bool hasNewSeasons);
	void setLastUpdateCheck(QDate lastUpdateCheck);

signals:
	void seasonStateChanged(QHash<SeasonType, int> seasonState);
	void totalSeasonCountChanged();
	void hasNewSeasonsChanged(bool hasNewSeasons);
	void lastUpdateCheckChanged(QDate lastUpdateCheck);

private:
	int _id;
	QString _title;

	QHash<SeasonType, int> _seasonState;
	bool _hasNewSeasons;
	QDate _lastUpdateCheck;

	mutable int _seasonCount;
};

typedef QList<AnimeInfo*> AnimeList;

Q_DECLARE_METATYPE(AnimeList)

#endif // ANIMEINFO_H
