#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H

#include "core_global.h"
#include "animeinfo.h"

#include <QObject>
#include <control.h>

class CORESHARED_EXPORT StatusControl : public Control
{
	Q_OBJECT

public:
	explicit StatusControl(QObject *parent = nullptr);

	void loadUpdateStatus(AnimeList animes);
	void loadErrorStatus(const QString &error);

	void onShow() override;

public slots:
	void showMainControl();

signals:
	void showUpdateNotification(bool success, const QString &title, const QString &message);

private:
	bool success;
	QString title;
	QString message;
};

#endif // STATUSCONTROL_H
