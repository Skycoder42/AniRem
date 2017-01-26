#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <quickpresenter.h>
#include <quickextras.h>
#include "cachingnamfactory.h"

#include "notifyingpresenter.h"
#include "maincontrol.h"
#include "addanimecontrol.h"

#include <QtAndroidExtras>

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("de.skycoder42"));
	QGuiApplication::setApplicationDisplayName(DISPLAY_NAME);
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	qmlRegisterUncreatableType<MainControl>("com.skycoder42.seasonproxer", 1, 0, "MainControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<AddAnimeControl>("com.skycoder42.seasonproxer", 1, 0, "AddAnimeControl", "Controls cannot be created!");

	auto parser = coreApp->getParser();
	if(parser && parser->isSet("update")) {
		QuickPresenter::registerAsPresenter<NotifyingPresenter>();
	} else {
		auto engine = QuickPresenter::createWithEngine<NotifyingPresenter>(QUrl());
		engine->setNetworkAccessManagerFactory(new CachingNamFactory());
		engine->load(QUrl(QLatin1String("qrc:///qml/App.qml")));

		//DEBUG
		QAndroidJniObject::callStaticMethod<void>("de/skycoder42/seasonproxer/Notifier",
												  "startService",
												  "(Landroid/content/Context;)V",
												  QtAndroid::androidContext().object());
	}

	return app.exec();
}
