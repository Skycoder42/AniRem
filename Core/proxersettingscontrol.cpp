#include "proxersettingscontrol.h"
#include <QStandardPaths>
#include <coremessage.h>
#include <setup.h>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

#include <QDebug>
#include <QUuid>

ProxerSettingsControl::ProxerSettingsControl(QObject *parent) :
	SettingsControl(parent),
	authenticator(QtDataSync::Setup::authenticatorForSetup<QtDataSync::WsAuthenticator>(this))
{
	connect(this, &ProxerSettingsControl::autoCheckChanged,
			this, &ProxerSettingsControl::updateAutoStart);
}

void ProxerSettingsControl::ensureAutoStart()
{
	QMetaObject::invokeMethod(this, "updateAutoStart", Qt::QueuedConnection,
							  Q_ARG(int, autoCheck()));
}

QVariant ProxerSettingsControl::loadValue(const QString &uiId, const QVariant &defaultValue) const
{
	if(uiId.startsWith(QStringLiteral("sync/"))) {
		if(uiId == QStringLiteral("sync/sync")){
			Q_UNIMPLEMENTED();//TODO use real property
			return true;//TODO use real property
		} else if(uiId == QStringLiteral("sync/user"))
			return authenticator->userIdentity();
		else
			return defaultValue;
	} else
		return SettingsControl::loadValue(uiId, defaultValue);
}

void ProxerSettingsControl::saveValue(const QString &uiId, const QVariant &value)
{
	if(uiId.startsWith(QStringLiteral("sync/"))) {
		if(uiId == QStringLiteral("sync/sync"))
			Q_UNIMPLEMENTED();//TODO use real property
		else if(uiId == QStringLiteral("sync/user"))
			authenticator->setUserIdentity(QUuid(value.toString()).toByteArray());
	} else
		SettingsControl::saveValue(uiId, value);
}

void ProxerSettingsControl::resetValue(const QString &uiId)
{
	if(uiId.startsWith(QStringLiteral("sync/"))) {
		if(uiId == QStringLiteral("sync/sync"))
			Q_UNIMPLEMENTED();//TODO use real property
		else if(uiId == QStringLiteral("sync/user"))
			authenticator->resetUserIdentity();
	} else
		SettingsControl::resetValue(uiId);
}

void ProxerSettingsControl::updateAutoStart(int interval)
{
	bool autoStart = interval > 0;
	if(!setAutoStart(autoStart)) {
		if(autoStart) {
			CoreMessage::information(tr("Enable autostart"),
									 tr("For automatic update checks to work, you need to add this application as "
										"an autostart application (for this user)."));
		} else {
			CoreMessage::information(tr("Disable autostart"),
									 tr("Since automatic update check has been disabled, you can safely remove "
										"this application form autostart."));
		}
	}
}

bool ProxerSettingsControl::setAutoStart(bool autoStart)
{
#if defined(Q_OS_WIN)
	QSettings regEdit(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	if(regEdit.isWritable()) {
		if(autoStart != regEdit.contains(QCoreApplication::applicationName())) {
			if(autoStart) {
				auto path = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
				regEdit.setValue(QCoreApplication::applicationName(),
								 QStringLiteral("\"%1\" --update").arg(path));
				return true;
			} else {
				regEdit.remove(QCoreApplication::applicationName());
				return true;
			}
		} else
			return true;
	}
#elif defined(Q_OS_ANDROID)
	auto activity = QtAndroid::androidActivity();
	if(activity.isValid()) {
		QtAndroid::runOnAndroidThread([=](){
			QAndroidJniObject::callStaticMethod<void>("de/skycoder42/seasonproxer/AlarmReceiver",
													  "scheduleAutoCheck",
													  "(Landroid/content/Context;Z)V",
													  activity.object(),
													  (jboolean)autoStart);
		});
		return true;
	}
#elif defined(Q_OS_LINUX)
	//TODO won't work this way, because the desktop file needs the --update parameter
	auto desktopFilePath = QStringLiteral("/usr/share/applications/%1.desktop")
						   .arg(QCoreApplication::applicationName());
	auto linkPath = QStringLiteral("%1/autostart/%2.desktop")
					.arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
					.arg(QCoreApplication::applicationName());
	if(autoStart) {
		if(QFile::link(desktopFilePath, linkPath))
			return true;
	} else {
		if(QFile::remove(linkPath))
			return true;
	}
#endif

	auto didNotify = settings()->value(QStringLiteral("updates/didNotify"), false);
	settings()->setValue(QStringLiteral("updates/didNotify"), autoStart);
	return autoStart == didNotify;
}
