#ifndef DETAILSCONTROL_H
#define DETAILSCONTROL_H

#include "core_global.h"
#include "animeinfo.h"
#include "animestore.h"

#include <QObject>
#include <control.h>

class CORESHARED_EXPORT DetailsControl : public Control
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
