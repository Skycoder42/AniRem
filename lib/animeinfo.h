#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include <QObject>
#include <QSharedDataPointer>
#include <QDateTime>
#include <QMap>
#include <QUrl>
#include <QCoreApplication>
#include "lib_anirem_global.h"

class AnimeInfoData;
class LIB_ANIREM_EXPORT AnimeInfo
{
	Q_GADGET
	Q_DECLARE_TR_FUNCTIONS(AnimeInfo)

	Q_PROPERTY(int id READ id WRITE setId USER true)
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(SeasonMap seasonState READ seasonState WRITE setSeasonState)
	Q_PROPERTY(int totalSeasonCount READ totalSeasonCount STORED false)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons STORED false)
	Q_PROPERTY(QDateTime lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck)
	Q_PROPERTY(QUrl relationsUrl READ relationsUrl CONSTANT STORED false)

public:
	enum SeasonType {
		Anime,
		Ova,
		Movie,
		Hentai,
		Manga,
		Oneshot,
		Doujin,
		Hmanga,
		Unknown
	};
	Q_ENUM(SeasonType)

	using SeasonInfo = QPair<int, bool>; //(count, changed)
	using SeasonMap = QMap<SeasonType, SeasonInfo>;

	Q_INVOKABLE AnimeInfo();
	AnimeInfo(int id, const QString &title);
	AnimeInfo(const AnimeInfo &other);
	AnimeInfo &operator=(const AnimeInfo &other);
	~AnimeInfo();

	operator bool() const;
	bool operator!() const;

	int id() const;
	QString title() const;
	SeasonMap seasonState() const;
	SeasonInfo seasonInfo(SeasonType type) const;
	int totalSeasonCount() const;
	bool hasNewSeasons() const;
	QDateTime lastUpdateCheck() const;
	QUrl relationsUrl() const;

	void setSeasonState(const SeasonMap &seasonState);
	void setLastUpdateCheck(QDateTime lastUpdateCheck);

	Q_INVOKABLE void setAllUnchanged();
	Q_INVOKABLE void setSeasonInfo(SeasonType type, SeasonInfo info);

	Q_INVOKABLE static QString typeToString(SeasonType type);

	bool operator==(const AnimeInfo &other) const;
	bool operator!=(const AnimeInfo &other) const;

private:
	QSharedDataPointer<AnimeInfoData> d;

	void setId(int id);
	void setTitle(const QString &title);
};

Q_DECLARE_METATYPE(AnimeInfo::SeasonInfo)
Q_DECLARE_METATYPE(AnimeInfo::SeasonMap)

#endif // ANIMEINFO_H
