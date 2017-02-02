#include "proxersettingscontrol.h"
#include <coremessage.h>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

ProxerSettingsControl::ProxerSettingsControl(QObject *parent) :
	SettingsControl(parent)
{
	connect(this, &ProxerSettingsControl::autoCheckChanged,
			this, &ProxerSettingsControl::updateAutoStart);
}

void ProxerSettingsControl::ensureAutoStart()
{
	QMetaObject::invokeMethod(this, "updateAutoStart", Qt::QueuedConnection,
							  Q_ARG(int, autoCheck()));
}

void ProxerSettingsControl::updateAutoStart(int interval)
{
	bool autoStart = interval > 0;
	if(!setAutoStart(autoStart)) {
		qDebug("here");
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
	//TODO support kde?
#endif

	auto didNotify = settings()->value(QStringLiteral("updates/didNotify"), false);
	settings()->setValue(QStringLiteral("updates/didNotify"), autoStart);
	return autoStart == didNotify;
}
