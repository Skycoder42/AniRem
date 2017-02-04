#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include "core_global.h"
#include <QDate>
#include <QMap>
#include <QObject>
#include <QPixmap>
#include <QSharedPointer>
#include <QUrl>

class CORESHARED_EXPORT AnimeInfo : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id CONSTANT)
	Q_PROPERTY(QString title READ title CONSTANT)
	Q_PROPERTY(QMap<SeasonType, SeasonInfo> seasonState READ seasonState WRITE setSeasonState NOTIFY seasonStateChanged)
	Q_PROPERTY(int totalSeasonCount READ totalSeasonCount NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(QDate lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck NOTIFY lastUpdateCheckChanged)
	Q_PROPERTY(QUrl relationsUrl READ relationsUrl CONSTANT STORED false)

public:
	typedef QPair<int, bool> SeasonInfo;

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

	explicit AnimeInfo(int id, const QString &title, QObject *parent = nullptr);

	int id() const;
	QString title() const;
	QMap<SeasonType, SeasonInfo> seasonState() const;
	SeasonInfo seasonInfo(SeasonType type) const;
	int totalSeasonCount() const;
	bool hasNewSeasons() const;
	QDate lastUpdateCheck() const;

	QUrl relationsUrl() const;

	void setAllUnchanged();
	static QString typeToString(SeasonType type);

public slots:
	void setSeasonState(QMap<SeasonType, SeasonInfo> seasonState);
	void setSeasonInfo(SeasonType type, SeasonInfo info);
	void setLastUpdateCheck(QDate lastUpdateCheck);

	void openUrl();

signals:
	void seasonStateChanged();
	void lastUpdateCheckChanged(QDate lastUpdateCheck);

private:
	int _id;
	QString _title;

	QMap<SeasonType, SeasonInfo> _seasonState;
	QDate _lastUpdateCheck;

	mutable int _seasonCount;
	mutable int _hasNewSeasons;
};

typedef QList<AnimeInfo*> AnimeList;

Q_DECLARE_METATYPE(AnimeList)

#endif // ANIMEINFO_H
