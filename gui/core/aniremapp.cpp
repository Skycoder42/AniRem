#include "aniremapp.h"
#include "mainviewmodel.h"

#include <QtCore/QCommandLineParser>

AniRemApp::AniRemApp(QObject *parent) :
	CoreApp(parent)
{
	QCoreApplication::setApplicationName(QStringLiteral("gui"));
	QCoreApplication::setApplicationVersion(QStringLiteral("1.0.0"));
	QCoreApplication::setOrganizationName(QStringLiteral("Example Organization"));
}

void AniRemApp::performRegistrations()
{
	//if you are using a qt resource (e.g. "guicore.qrc"), initialize it here
	Q_INIT_RESOURCE(anirem_core);
}

int AniRemApp::startApp(const QStringList &arguments)
{
	QCommandLineParser parser;
	parser.addVersionOption();
	parser.addHelpOption();

	//add more options

	//shows help or version automatically
	if(!autoParse(parser, arguments))
		return EXIT_SUCCESS;

	//show a viewmodel to complete the startup
	show<MainViewModel>();
	return EXIT_SUCCESS;
}
