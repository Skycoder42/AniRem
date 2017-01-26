#include "statusview.h"
#include <QApplication>

StatusView::StatusView(StatusControl *control, QObject *parent) :
	QObject(parent),
	control(static_cast<StatusControl*>(control)),
	trayIcon(new QSystemTrayIcon(QApplication::windowIcon(), this)),
	trayMenu(new QMenu()),
	blinkTimer(new QTimer(this)),
	normalIcon(QStringLiteral(":/icons/main.ico")),
	invertIcon(QStringLiteral(":/icons/inverted.ico")),
	currentNormal(true)
{
	connect(control, &StatusControl::showUpdateNotification,
			this, &StatusView::showUpdateNotification);

	trayMenu->addAction(QIcon::fromTheme(QStringLiteral("gtk-execute")), tr("Show Main Window"), control, &StatusControl::showMainControl);
	trayMenu->addAction(QIcon::fromTheme(QStringLiteral("gtk-quit")), tr("Quit"), qApp, &QApplication::quit);
	trayIcon->setContextMenu(trayMenu.data());

	trayIcon->setToolTip(QApplication::applicationDisplayName());
	connect(trayIcon, &QSystemTrayIcon::activated,
			this, &StatusView::trayActivated);
	connect(trayIcon, &QSystemTrayIcon::messageClicked,
			control, &StatusControl::showMainControl);

	blinkTimer->setInterval(500);
	connect(blinkTimer, &QTimer::timeout,
			this, &StatusView::timerTriggered);
}

void StatusView::showUpdateNotification(bool success, const QString &title, const QString &message)
{
	trayIcon->show();
	blinkTimer->start();
	trayIcon->showMessage(title, message, success ? QSystemTrayIcon::Information : QSystemTrayIcon::Critical);
}

void StatusView::timerTriggered()
{
	if(currentNormal)
		trayIcon->setIcon(invertIcon);
	else
		trayIcon->setIcon(normalIcon);
	currentNormal = !currentNormal;
}

void StatusView::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::DoubleClick:
	case QSystemTrayIcon::MiddleClick:
	case QSystemTrayIcon::Trigger:
		control->showMainControl();
		break;
	default:
		break;
	}
}
