#include "aniremapp.h"
#include "mainviewmodel.h"

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QtMvvmCore/ServiceRegistry>
#include <libanirem.h>

#include "imageloader.h"

AniRemApp::AniRemApp(QObject *parent) :
	CoreApp(parent)
{
	QCoreApplication::setApplicationName(QStringLiteral("anirem"));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QCoreApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setApplicationDisplayName(QStringLiteral("Ani-Rem"));
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral("")));
}

void AniRemApp::performRegistrations()
{
	Q_INIT_RESOURCE(anirem_core);
	AniRem::prepareTranslations();

	QtMvvm::ServiceRegistry::instance()->registerObject<ImageLoader>();
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

	//show a viewmodel to complete the startup
	show<MainViewModel>();
	return EXIT_SUCCESS;
}
