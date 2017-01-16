#ifndef ANIMEINFO_H
#define ANIMEINFO_H

#include "core_global.h"
#include <QObject>
#include <QPixmap>
#include <QSharedPointer>
#include <QUrl>

class CORESHARED_EXPORT AnimeInfo : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id CONSTANT)
	Q_PROPERTY(QString title READ id CONSTANT)
	Q_PROPERTY(int lastKnownSeasons READ lastKnownSeasons WRITE setLastKnownSeasons NOTIFY lastKnownSeasonsChanged)
	Q_PROPERTY(bool hasNewSeasons READ hasNewSeasons WRITE setHasNewSeasons NOTIFY hasNewSeasonsChanged)

public:
	explicit AnimeInfo(int id, const QString &title, QObject *parent = nullptr);

	int id() const;
	QString title() const;
	int lastKnownSeasons() const;
	bool hasNewSeasons() const;

	QUrl relationsUrl() const;

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

typedef QList<AnimeInfo*> AnimeList;

Q_DECLARE_METATYPE(AnimeList)

#endif // ANIMEINFO_H
