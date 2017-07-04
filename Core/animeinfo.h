#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QPixmap>
#include <QSharedPointer>
#include <QUrl>

class AnimeInfo : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id MEMBER _id READ id USER true)
	Q_PROPERTY(QString title MEMBER _title READ title)
	Q_PROPERTY(QMap<AnimeInfo::SeasonType, AnimeInfo::SeasonInfo> seasonState READ seasonState WRITE setSeasonState NOTIFY seasonStateChanged)
	Q_PROPERTY(int totalSeasonCount READ totalSeasonCount NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons NOTIFY seasonStateChanged STORED false)
	Q_PROPERTY(QDateTime lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck NOTIFY lastUpdateCheckChanged)
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
	Q_INVOKABLE explicit AnimeInfo(QObject *parent = nullptr);

	int id() const;
	QString title() const;
	QMap<SeasonType, SeasonInfo> seasonState() const;
	SeasonInfo seasonInfo(SeasonType type) const;
	int totalSeasonCount() const;
	bool hasNewSeasons() const;
	QDateTime lastUpdateCheck() const;

	QUrl relationsUrl() const;

	void setAllUnchanged();
	static QString typeToString(SeasonType type);

public slots:
	void setSeasonState(QMap<SeasonType, SeasonInfo> seasonState);
	void setSeasonInfo(SeasonType type, SeasonInfo info);
	void setLastUpdateCheck(QDateTime lastUpdateCheck);

	void openUrl();

signals:
	void seasonStateChanged();
	void lastUpdateCheckChanged(QDateTime lastUpdateCheck);

private:
	int _id;
	QString _title;

	QMap<SeasonType, SeasonInfo> _seasonState;
	QDateTime _lastUpdateCheck;

	mutable int _seasonCount;
	mutable int _hasNewSeasons;
};

Q_DECLARE_METATYPE(AnimeInfo::SeasonInfo)

#endif // ANIMEINFO_H
