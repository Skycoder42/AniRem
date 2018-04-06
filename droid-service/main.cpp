#include <QAndroidService>
#include <QDebug>
#include <libanirem.h>

int main(int argc, char *argv[])
{
	QAndroidService service(argc, argv);
	//WORKAROUND
	qputenv("PLUGIN_KEYSTORES_PATH", QCoreApplication::applicationDirPath().toUtf8());
	qInfo() << "Overwriting keystore path to:" << qgetenv("PLUGIN_KEYSTORES_PATH");

	//load translations
	AniRem::prepareTranslations();

	try {
		QtDataSync::Setup setup;
		AniRem::setup(setup, true);
		setup.create();
		qInfo() << "service successfully started";

		return service.exec();
	} catch(QException &e) {
		qCritical() << e.what();
		return EXIT_FAILURE;
	}
}
