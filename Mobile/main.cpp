#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <quickpresenter.h>
#include <quickextras.h>
#include "cachingnamfactory.h"

#include "maincontrol.h"
#include "addanimecontrol.h"

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QGuiApplication::setApplicationDisplayName(DISPLAY_NAME);
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	QuickPresenter::registerAsPresenter();
	QuickExtras::registerQmlSingleton();

	qmlRegisterUncreatableType<MainControl>("com.skycoder42.seasonproxer", 1, 0, "MainControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<AddAnimeControl>("com.skycoder42.seasonproxer", 1, 0, "AddAnimeControl", "Controls cannot be created!");

	QQmlApplicationEngine engine;
	QuickExtras::setupEngine(&engine);
	engine.setNetworkAccessManagerFactory(new CachingNamFactory());
	engine.load(QUrl(QLatin1String("qrc:///qml/App.qml")));

	return app.exec();
}
