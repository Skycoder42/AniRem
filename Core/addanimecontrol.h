#ifndef ADDANIMECONTROL_H
#define ADDANIMECONTROL_H

#include "core_global.h"

#include <QObject>
#include <control.h>
#include "animeinfo.h"
class InfoClass;

class CORESHARED_EXPORT AddAnimeControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
	Q_PROPERTY(bool acceptable READ isAcceptable NOTIFY acceptableChanged)

public:
	explicit AddAnimeControl(QObject *parent = nullptr);

	int id() const;
	QString title() const;
	bool isLoading() const;
	bool isAcceptable() const;

public slots:
	void accept(bool allowInvalid);
	void retry();

	void setId(int id);
	void setTitle(QString title);

signals:
	void loadError(const QString &error);
	void completed(AnimeInfo *animeInfo);

	void idChanged(int id);
	void titleChanged(QString title);
	void loadingChanged(bool loading);
	void acceptableChanged(bool acceptable);

private:
	InfoClass *infoClass;

	int _id;
	QString _title;
	bool _loading;
	bool _acceptable;

	void setLoading(bool loading);
	void setAcceptable(bool acceptable);
};

#endif // ADDANIMECONTROL_H
