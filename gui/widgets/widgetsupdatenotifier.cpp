#include "widgetsupdatenotifier.h"

#include <QApplication>
#include <aniremapp.h>
#include <mainviewmodel.h>
#include "instancesetup.h"

WidgetsUpdateNotifier::WidgetsUpdateNotifier(QObject *parent) :
	QObject(parent),
	_trayIcon(new QSystemTrayIcon(QApplication::windowIcon(), this)),
	_trayMenu(new QMenu()),
	_blinkTimer(new QTimer(this)),
	_normalIcon(QStringLiteral(":/icons/main.ico")),
	_invertIcon(QStringLiteral(":/icons/inverted.ico")),
	_currentNormal(true)
{
	_trayMenu->addAction(QIcon::fromTheme(QStringLiteral("gtk-execute")), tr("Show Main Window"),
						 this, &WidgetsUpdateNotifier::showMainViewModel);
	_trayMenu->addAction(QIcon::fromTheme(QStringLiteral("gtk-quit")), tr("Quit"),
						 this, &WidgetsUpdateNotifier::quit);
	_trayIcon->setContextMenu(_trayMenu.data());

	_trayIcon->setToolTip(QApplication::applicationDisplayName());
	connect(_trayIcon, &QSystemTrayIcon::activated,
			this, &WidgetsUpdateNotifier::trayActivated);
	connect(_trayIcon, &QSystemTrayIcon::messageClicked,
			this, &WidgetsUpdateNotifier::showMainViewModel);

	_blinkTimer->setInterval(500);
	connect(_blinkTimer, &QTimer::timeout,
			this, &WidgetsUpdateNotifier::timerTriggered);
}

void WidgetsUpdateNotifier::showNotification(bool isPositive, const QString &title, const QString &description)
{
	_trayIcon->show();
	_blinkTimer->start();
	_trayIcon->showMessage(title, description, isPositive ? QSystemTrayIcon::Information : QSystemTrayIcon::Critical);
}

void WidgetsUpdateNotifier::updateProgress(int value, int max)
{
	Q_UNUSED(value)
	Q_UNUSED(max)
}

void WidgetsUpdateNotifier::timerTriggered()
{
	if(_currentNormal)
		_trayIcon->setIcon(_invertIcon);
	else
		_trayIcon->setIcon(_normalIcon);
	_currentNormal = !_currentNormal;
	if(getMainWindow()) {
		_trayIcon->hide();
		_blinkTimer->stop();
	}
}

void WidgetsUpdateNotifier::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::DoubleClick:
	case QSystemTrayIcon::Trigger:
		showMainViewModel();
		break;
	default:
		break;
	}
}

void WidgetsUpdateNotifier::showMainViewModel()
{
	_trayIcon->hide();
	_blinkTimer->stop();
	coreApp->show<MainViewModel>();
}

void WidgetsUpdateNotifier::quit()
{
	_trayIcon->hide();
	_blinkTimer->stop();
	qApp->quit();
}
