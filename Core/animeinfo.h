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
	Q_PROPERTY(QHash<SeasonType, SeasonInfo> seasonState READ seasonState WRITE setSeasonState NOTIFY seasonStateChanged)
	Q_PROPERTY(int totalSeasonCount READ totalSeasonCount NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(QDate lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck NOTIFY lastUpdateCheckChanged)
	Q_PROPERTY(QUrl relationsUrl READ relationsUrl CONSTANT STORED false)

public:
	typedef QPair<int, bool> SeasonInfo;

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
	QHash<SeasonType, SeasonInfo> seasonState() const;
	SeasonInfo seasonInfo(SeasonType type) const;
	int totalSeasonCount() const;
	bool hasNewSeasons() const;
	QDate lastUpdateCheck() const;

	QUrl relationsUrl() const;

public slots:
	void setSeasonState(QHash<SeasonType, SeasonInfo> seasonState);
	void setSeasonInfo(SeasonType type, SeasonInfo info);
	void setAllUnchanged();
	void setLastUpdateCheck(QDate lastUpdateCheck);

signals:
	void seasonStateChanged();
	void lastUpdateCheckChanged(QDate lastUpdateCheck);

private:
	int _id;
	QString _title;

	QHash<SeasonType, SeasonInfo> _seasonState;
	QDate _lastUpdateCheck;

	mutable int _seasonCount;
	mutable int _hasNewSeasons;
};

typedef QList<AnimeInfo*> AnimeList;

Q_DECLARE_METATYPE(AnimeList)

#endif // ANIMEINFO_H
