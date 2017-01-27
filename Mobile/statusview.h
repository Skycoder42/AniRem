#ifndef STATUSVIEW_H
#define STATUSVIEW_H

#include <QObject>
#include <statuscontrol.h>
#include <QAndroidJniObject>

class StatusView : public QObject
{
	Q_OBJECT

public:
	explicit StatusView(Control *mControl, QObject *parent = nullptr);

private slots:
	void updateProgress(int current, int max);
	void showUpdateNotification(bool success, const QString &title, const QString &message);

private:
	StatusControl *control;
	QAndroidJniObject service;
};

#endif // STATUSVIEW_H
