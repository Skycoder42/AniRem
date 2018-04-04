#include "aniremapp.h"
#include "mainviewmodel.h"

#include <QCommandLineParser>
#include <QFile>
#include <QGuiApplication>
#include <QIcon>
#include <QtMvvmCore/ServiceRegistry>
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <libanirem.h>
#include <seasonstatusloader.h>
#include <syncedsettings.h>
#include <localsettings.h>

#include "imageloader.h"

AniRemApp::AniRemApp(QObject *parent) :
	CoreApp(parent)
{
	QCoreApplication::setApplicationName(QStringLiteral("anirem"));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QCoreApplication::setOrganizationDomain(QStringLiteral(BUNDLE));
	QGuiApplication::setApplicationDisplayName(QStringLiteral("Ani-Rem"));
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral("")));
}

void AniRemApp::updateAutoStartState()
{
	auto interval = SyncedSettings::instance()->updates.autoCheck.get().toInt();
	if(!setAutoStart(interval > 0)) {
		if(interval > 0) {
			QtMvvm::information(tr("Enable autostart"),
								tr("For automatic update checks to work, you need to add this application as "
								   "an autostart application (for this user)."));
		} else {
			QtMvvm::information(tr("Disable autostart"),
								tr("Since automatic update check has been disabled, you can safely remove "
								   "this application form autostart."));
		}
	}
}

void AniRemApp::performRegistrations()
{
	Q_INIT_RESOURCE(anirem_core);
	AniRem::prepareTranslations();

	QtMvvm::ServiceRegistry::instance()->registerObject<ImageLoader>();
	QtMvvm::ServiceRegistry::instance()->registerObject<SeasonStatusLoader>();
}

int AniRemApp::startApp(const QStringList &arguments)
{
	QCommandLineParser parser;
	parser.addVersionOption();
	parser.addHelpOption();

	//add more options
	parser.addOption({
						 QStringLiteral("passive"),
						 tr("Start in passive mode. Useful only when running with an external backend service.")
					 });
	parser.addOption({
						 {QStringLiteral("u"), QStringLiteral("update")},
						 tr("Ani-Rem will start without a GUI, checking for updates passively")
					 });

	//shows help or version automatically
	if(!autoParse(parser, arguments))
		return EXIT_SUCCESS;

	try {
		auto passive = parser.isSet(QStringLiteral("passive"));
		QtDataSync::Setup setup;
		AniRem::setup(setup, passive);
		if(passive)
			setup.createPassive(QtDataSync::DefaultSetup, 5000); //TODO log
		else
			setup.create();
	} catch(QtDataSync::SetupLockedException &e) {
		qWarning() << e.what();
		qApp->quit();
		return EXIT_SUCCESS;
	}

	//update autostart state
	updateAutoStartState();

	//show a viewmodel to complete the startup
	show<MainViewModel>();
	return EXIT_SUCCESS;
}

bool AniRemApp::setAutoStart(bool autoStart)
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
			QAndroidJniObject::callStaticMethod<void>("de/skycoder42/anirem/AlarmReceiver",
													  "scheduleAutoCheck",
													  "(Landroid/content/Context;Z)V",
													  activity.object(),
													  (jboolean)autoStart);
		});
		return true;
	}
#elif defined(Q_OS_LINUX)
	auto autoStartPath = QStringLiteral("%1/autostart/%2.%3.desktop")
						 .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
						 .arg(QCoreApplication::organizationDomain())
						 .arg(QCoreApplication::applicationName());
	if(autoStart) {
		if(QFile::exists(autoStartPath))
			return true;
		else {
			QSettings autoStartFile(autoStartPath, QSettings::IniFormat);
			autoStartFile.beginGroup(QStringLiteral("Desktop_Entry"));
			autoStartFile.setValue(QStringLiteral("Type"), QStringLiteral("Application"));
			autoStartFile.setValue(QStringLiteral("Version"), QStringLiteral("1.1"));
			autoStartFile.setValue(QStringLiteral("Name"), QGuiApplication::applicationDisplayName());
			autoStartFile.setValue(QStringLiteral("Comment"), QGuiApplication::applicationDisplayName());
			autoStartFile.setValue(QStringLiteral("Exec"), QString(QCoreApplication::applicationFilePath() + QStringLiteral(" --update")));
			autoStartFile.setValue(QStringLiteral("Icon"), QCoreApplication::applicationName());
			autoStartFile.setValue(QStringLiteral("Terminal"), false);
			autoStartFile.endGroup();
			autoStartFile.sync();

			//fix the underscore to space
			QFile fixFile(autoStartPath);
			if(!fixFile.open(QIODevice::ReadWrite)) {
				fixFile.remove();
				return false;
			} else {
				fixFile.seek(8);
				fixFile.write(" ", 1);
				fixFile.close();
				return true;
			}
		}
	} else {
		if(QFile::remove(autoStartPath))
			return true;
	}
#endif


	bool didNotify = LocalSettings::instance()->updates.didNotify;
	LocalSettings::instance()->updates.didNotify = autoStart;
	return autoStart == didNotify;
}
