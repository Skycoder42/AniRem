#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <quickpresenter.h>
#include <quickextras.h>
#include <settingsinputviewfactory.h>
#include <proxerapp.h>
#include "cachingnamfactory.h"

#include "notifyingpresenter.h"
#include "maincontrol.h"
#include "addanimecontrol.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

REGISTER_CORE_APP(ProxerApp)

static bool isServer();
static void setStatusBarColor(); //TODO move to QuickExtras

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	if(!(argc > 1 && argv[1] == QByteArray("--update")))
		setStatusBarColor();

	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("de.skycoder42"));
	QGuiApplication::setApplicationDisplayName(DISPLAY_NAME);
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	qmlRegisterUncreatableType<MainControl>("de.skycoder42.seasonproxer", 1, 0, "MainControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<AddAnimeControl>("de.skycoder42.seasonproxer", 1, 0, "AddAnimeControl", "Controls cannot be created!");
	qmlRegisterUncreatableType<DetailsControl>("de.skycoder42.seasonproxer", 1, 0, "DetailsControl", "Controls cannot be created!");

	QuickPresenter::registerAsPresenter<NotifyingPresenter>();
	QuickPresenter::registerInputViewFactory(new SettingsInputViewFactory());
	if(!isServer()) {
		auto engine = QuickPresenter::createAppEngine(QUrl());
		engine->setNetworkAccessManagerFactory(new CachingNamFactory());
		engine->load(QUrl(QLatin1String("qrc:///qml/App.qml")));
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

static void setStatusBarColor()
{
#ifdef Q_OS_ANDROID
	if(QtAndroid::androidSdkVersion() >= 21) {
		QtAndroid::runOnAndroidThreadSync([=](){
			auto activity = QtAndroid::androidActivity();
			if(activity.isValid()) {
				const auto FLAG_TRANSLUCENT_STATUS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																							 "FLAG_TRANSLUCENT_STATUS");
				const auto FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																									   "FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS");
				const auto color = QAndroidJniObject::callStaticMethod<jint>("android/graphics/Color",
																			 "parseColor",
																			 "(Ljava/lang/String;)I",
																			 QAndroidJniObject::fromString("#4E4E4E").object());

				QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
				if(window.isValid()) {
					window.callMethod<void>("clearFlags", "(I)V", FLAG_TRANSLUCENT_STATUS);
					window.callMethod<void>("addFlags", "(I)V", FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
					window.callMethod<void>("setStatusBarColor", "(I)V", color);
				}
			}
		});
	}
#endif
}

