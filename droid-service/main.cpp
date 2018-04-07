#include <QAndroidService>
#include <QDebug>
#include <QMutex>
#include <QtMvvmCore/ServiceRegistry>
#include <libanirem.h>
#include "starthelper.h"
#include "androidupdatenotifier.h"

int main(int argc, char *argv[])
{
	QAndroidService service(argc, argv);
	//WORKAROUND until fixed in upstream
	qputenv("PLUGIN_KEYSTORES_PATH", QCoreApplication::applicationDirPath().toUtf8());
	qInfo() << "Overwriting keystore path to:" << qgetenv("PLUGIN_KEYSTORES_PATH");

	//load translations
	AniRem::prepareTranslations();

	QtMvvm::ServiceRegistry::instance()->registerInterface<IUpdateNotifier, AndroidUpdateNotifier>();

	try {
		QtDataSync::Setup setup;
		AniRem::setup(setup, true);
		setup.create();

		StartHelper starter;

		qInfo() << "service successfully started";
		return service.exec();
	} catch(QException &e) {
		qCritical() << e.what();
		return EXIT_FAILURE;
	}
}
