#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <quickpresenter.h>
#include <quickextras.h>
#include <androidutils.h>
#include <settingsinputviewfactory.h>
#include <proxerapp.h>

#include "notifyingpresenter.h"
#include "proxerimageprovider.h"
#include "maincontrol.h"
#include "addanimecontrol.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

REGISTER_CORE_APP(ProxerApp)

static bool isServer();

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	if(!(argc > 1 && argv[1] == QByteArray("--update")))
		AndroidUtils::instance()->setStatusBarColor(QColor(0x4E, 0x4E, 0x4E));

	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("de.skycoder42"));
	QGuiApplication::setApplicationDisplayName(DISPLAY_NAME);
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	QuickExtras::setHapticFeedbackProvider([](){
		AndroidUtils::instance()->hapticFeedback();
	});

	qmlRegisterUncreatableType<MainControl>("de.skycoder42.seasonproxer", 1, 0, "MainControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<AddAnimeControl>("de.skycoder42.seasonproxer", 1, 0, "AddAnimeControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<DetailsControl>("de.skycoder42.seasonproxer", 1, 0, "DetailsControl", "Controls cannot be created!");

	QuickPresenter::registerAsPresenter<NotifyingPresenter>();
	QuickPresenter::registerInputViewFactory(new SettingsInputViewFactory());
	if(!isServer()) {
		auto engine = QuickPresenter::createAppEngine(QUrl());
		engine->addImageProvider(QStringLiteral("proxer"), new ProxerImageProvider());
		engine->load(QUrl(QStringLiteral("qrc:///qml/App.qml")));
	}

	auto res = app.exec();
#ifdef Q_OS_ANDROID
	if(isServer())
		QAndroidJniObject::callStaticMethod<void>("java/lang/System", "exit", "(I)V", res);
#endif
	return res;
}

static bool isServer()
{
	auto parser = coreApp->getParser();
	return parser && parser->isSet("update");
}

