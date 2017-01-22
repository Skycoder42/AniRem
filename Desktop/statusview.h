#ifndef STATUSVIEW_H
#define STATUSVIEW_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QScopedPointer>
#include <statuscontrol.h>
#include <QMenu>

#include <C:/Qt/5.8/msvc2015_64/include/QtCore/qtimer.h>

class StatusView : public QObject
{
	Q_OBJECT

public:
	explicit StatusView(Control *mControl, QObject *parent = nullptr);

private slots:
	void showUpdateNotification(bool success, const QString &title, const QString &message);

	void timerTriggered();
	void trayActivated(QSystemTrayIcon::ActivationReason reason);

private:
	StatusControl *control;
	QSystemTrayIcon *trayIcon;
	QScopedPointer<QMenu, QScopedPointerDeleteLater> trayMenu;

	QTimer *blinkTimer;
	QIcon normalIcon;
	QIcon invertIcon;
	bool currentNormal;
};

#endif // STATUSVIEW_H
