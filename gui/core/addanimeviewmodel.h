#ifndef ADDANIMEVIEWMODEL_H
#define ADDANIMEVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <animeinfo.h>
#include <libanirem.h>
#include <infoclass.h>
#include <seasonstatusloader.h>
#include "imageloader.h"

class AddAnimeViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
	Q_PROPERTY(bool acceptable READ isAcceptable NOTIFY acceptableChanged)

	QTMVVM_INJECT_PROP(ImageLoader*, imageLoader, _loader)
	QTMVVM_INJECT_PROP(SeasonStatusLoader*, updater, _updater)

public:
	static const QString ParamId;

	static QVariantHash params(int id);

	Q_INVOKABLE explicit AddAnimeViewModel(QObject *parent = nullptr);

	int id() const;
	QString title() const;
	bool isLoading() const;
	bool isAcceptable() const;

	ImageLoader* imageLoader() const;

public slots:
	bool accept(bool allowInvalid);

	void setId(int id);
	void setTitle(QString title);

signals:
	void close();

	void idChanged(int id);
	void titleChanged(QString title);
	void loadingChanged(bool loading);
	void acceptableChanged(bool acceptable);

protected:
	void onInit(const QVariantHash &params) override;

private slots:
	void error(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType);

private:
	AniremStore *_store;
	InfoClass *_infoClass;
	ImageLoader *_loader;
	SeasonStatusLoader *_updater;

	int _id;
	QString _title;
	bool _loading;
	bool _acceptable;

	void setLoading(bool loading);
	void setAcceptable(bool acceptable);
};

#endif // ADDANIMEVIEWMODEL_H
