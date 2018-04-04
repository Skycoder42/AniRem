#ifndef ADDANIMECONTROL_H
#define ADDANIMECONTROL_H

#include <QtMvvmCore/ViewModel>
#include <animeinfo.h>
#include "imageloader.h"
class InfoClass;

class AddAnimeViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
	Q_PROPERTY(bool acceptable READ isAcceptable NOTIFY acceptableChanged)

	QTMVVM_INJECT_PROP(ImageLoader*, imageLoader, _loader)

public:
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

private slots:
	void error(const QString &errorString, int errorCode, int errorType);

private:
	InfoClass *_infoClass;
	ImageLoader *_loader;

	int _id;
	QString _title;
	bool _loading;
	bool _acceptable;

	void setLoading(bool loading);
	void setAcceptable(bool acceptable);
};

#endif // ADDANIMECONTROL_H
