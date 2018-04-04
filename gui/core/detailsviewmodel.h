#ifndef DETAILSVIEWMODEL_H
#define DETAILSVIEWMODEL_H

#include <QtMvvmCore/ViewModel>

#include <libanirem.h>
#include "imageloader.h"

class DetailsViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(AnimeInfo animeInfo READ animeInfo WRITE setAnimeInfo NOTIFY animeInfoChanged)
	Q_PROPERTY(QString detailsText READ detailsText NOTIFY animeInfoChanged)

	QTMVVM_INJECT_PROP(ImageLoader*, imageLoader, _loader)

public:
	static const QString ParamInfo;
	static const QString ParamOwner;

	static QVariantHash params(const AnimeInfo &info, QtMvvm::ViewModel *owner = nullptr);

	Q_INVOKABLE explicit DetailsViewModel(QObject *parent = nullptr);

	AnimeInfo animeInfo() const;
	QString detailsText() const;

	ImageLoader* imageLoader() const;

public slots:
	void setAnimeInfo(const AnimeInfo &animeInfo);
	void clear();

	void uncheckAnime();

signals:
	void animeInfoChanged();

protected:
	void onInit(const QVariantHash &params) override;

private:
	AniremStore *_store;
	ImageLoader *_loader;

	AnimeInfo _animeInfo;
};

#endif // DETAILSVIEWMODEL_H
