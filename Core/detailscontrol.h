#ifndef DETAILSCONTROL_H
#define DETAILSCONTROL_H

#include "animeinfo.h"

#include <QObject>
#include <control.h>

class DetailsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(AnimeInfo* animeInfo READ animeInfo WRITE setAnimeInfo NOTIFY animeInfoChanged)

public:
	explicit DetailsControl(QObject *parent = nullptr);

	AnimeInfo* animeInfo() const;

public slots:

	void setAnimeInfo(AnimeInfo* animeInfo);

signals:

	void animeInfoChanged(AnimeInfo* animeInfo);

private:
	AnimeInfo* _animeInfo;
};

#endif // DETAILSCONTROL_H
