#ifndef STATUSCONTROL_H
#define STATUSCONTROL_H

#include "animeinfo.h"

#include <QObject>
#include <control.h>

class StatusControl : public Control
{
	Q_OBJECT

public:
	explicit StatusControl(QObject *parent = nullptr);

	void loadUpdateStatus(QList<AnimeInfo*> animes);
	void loadErrorStatus(const QString &error);

public slots:
	void showMainControl();

signals:
	void updateProgress(int current, int max);
	void showUpdateNotification(bool success, const QString &title, const QString &message);
};

#endif // STATUSCONTROL_H
