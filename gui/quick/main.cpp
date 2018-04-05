#include "proxerimageprovider.h"

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtMvvmDataSyncQuick/qtmvvmdatasyncquick_global.h>
#include <aniremapp.h>

#include <mainviewmodel.h>
#include <addanimeviewmodel.h>
#include <detailsviewmodel.h>

QTMVVM_REGISTER_CORE_APP(AniRemApp)

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	// If you want to support file dialogs on platforms other then android, use a QApplication instead (and add QT += widgets to the pro file)
	QGuiApplication app(argc, argv);

	QtMvvm::registerDataSyncQuick();
	qmlRegisterUncreatableType<MainViewModel>("de.skycoder42.anirem", 1, 0, "MainViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<AddAnimeViewModel>("de.skycoder42.anirem", 1, 0, "AddAnimeViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<DetailsViewModel>("de.skycoder42.anirem", 1, 0, "DetailsViewModel", QStringLiteral("ViewModels cannot be created!"));

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/qml/App.qml")));
	engine.addImageProvider(QStringLiteral("proxer"), new ProxerImageProvider());
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
