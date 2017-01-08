#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include <QObject>
#include <QPixmap>
#include <QSharedPointer>

class AnimeInfo : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id CONSTANT)
	Q_PROPERTY(QString title READ id CONSTANT)
	Q_PROPERTY(int lastKnownSeasons READ lastKnownSeasons WRITE setLastKnownSeasons NOTIFY lastKnownSeasonsChanged)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons WRITE setHasNewSeasons NOTIFY hasNewSeasonsChanged)

public:
	explicit AnimeInfo(int id = -1, const QString &title = {});

	int id() const;
	QString title() const;
	int lastKnownSeasons() const;
	bool hasNewSeasons() const;

public slots:
	void setLastKnownSeasons(int lastKnownSeasons);
	void setHasNewSeasons(bool hasNewSeasons);

signals:
	void lastKnownSeasonsChanged(int lastKnownSeasons);
	void hasNewSeasonsChanged(bool hasNewSeasons);

private:
	int _id;
	QString _title;

	int _lastKnownSeasons;
	bool _hasNewSeasons;
};

typedef QSharedPointer<AnimeInfo> AnimePtr;
typedef QList<AnimePtr> AnimeList;

#endif // ANIMEINFO_H
