#ifndef WIDGETSUPDATENOTIFIER_H
#define WIDGETSUPDATENOTIFIER_H

#include <QMenu>
#include <QObject>
#include <QScopedPointer>
#include <QSystemTrayIcon>
#include <QTimer>
#include <iupdatenotifier.h>

class WidgetsUpdateNotifier : public QObject, public IUpdateNotifier
{
	Q_OBJECT
	Q_INTERFACES(IUpdateNotifier)

public:
	Q_INVOKABLE explicit WidgetsUpdateNotifier(QObject *parent = nullptr);

public slots:
	void showNotification(bool isPositive, const QString &title, const QString &description) override;

private slots:
	void timerTriggered();
	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void showMainViewModel();
	void quit();

private:
	QSystemTrayIcon *_trayIcon;
	QScopedPointer<QMenu, QScopedPointerDeleteLater> _trayMenu;

	QTimer *_blinkTimer;
	QIcon _normalIcon;
	QIcon _invertIcon;
	bool _currentNormal;
};

#endif // WIDGETSUPDATENOTIFIER_H
