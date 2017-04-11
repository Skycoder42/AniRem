#ifndef DETAILSCONTROL_H
#define DETAILSCONTROL_H

#include "animeinfo.h"
#include "proxerapp.h"

#include <QObject>
#include <control.h>

class DetailsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(AnimeInfo* animeInfo READ animeInfo WRITE setAnimeInfo NOTIFY animeInfoChanged)
	Q_PROPERTY(QString detailsText READ detailsText NOTIFY animeInfoChanged)

public:
	explicit DetailsControl(AnimeStore *store, QObject *parent = nullptr);

	AnimeInfo* animeInfo() const;
	QString detailsText() const;

public slots:
	void setAnimeInfo(AnimeInfo* animeInfo);
	void uncheckAnime();

signals:
	void animeInfoChanged();

private:
	AnimeStore *_store;
	AnimeInfo* _animeInfo;
};

#endif // DETAILSCONTROL_H
