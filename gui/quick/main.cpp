#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtMvvmDataSyncQuick/qtmvvmdatasyncquick_global.h>
#include <aniremapp.h>

#include <mainviewmodel.h>
#include <addanimeviewmodel.h>
#include <detailsviewmodel.h>
#include <entryviewmodel.h>
#include <loginviewmodel.h>

#include "proxerimageprovider.h"
#include "qmltoast.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

QTMVVM_REGISTER_CORE_APP(AniRemApp)

namespace {

void setStatusBarColor(QColor color)
{
#ifdef Q_OS_ANDROID
	if(QtAndroid::androidSdkVersion() >= 21) {
		auto activity = QtAndroid::androidActivity();
		QtAndroid::runOnAndroidThreadSync([=](){
			const auto FLAG_TRANSLUCENT_STATUS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																						 "FLAG_TRANSLUCENT_STATUS");
			const auto FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																								   "FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS");
			const auto jColor = QAndroidJniObject::callStaticMethod<jint>("android/graphics/Color",
																		  "parseColor",
																		  "(Ljava/lang/String;)I",
																		  QAndroidJniObject::fromString(color.name()).object());

			QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
			if(window.isValid()) {
				window.callMethod<void>("clearFlags", "(I)V", FLAG_TRANSLUCENT_STATUS);
				window.callMethod<void>("addFlags", "(I)V", FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
				window.callMethod<void>("setStatusBarColor", "(I)V", jColor);
			}
		});
	}
#else
	Q_UNUSED(color);
#endif
}

QObject *createQmlToast(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
	Q_UNUSED(jsEngine)
	return new QmlToast(qmlEngine);
}

}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	setStatusBarColor(QColor(0x4E, 0x4E, 0x4E));
	// If you want to support file dialogs on platforms other then android, use a QApplication instead (and add QT += widgets to the pro file)
	QGuiApplication app(argc, argv);

	qmlRegisterSingletonType<QmlToast>("de.skycoder42.anirem", 1, 0, "QmlToast", createQmlToast);

	QtMvvm::registerDataSyncQuick();
	qmlRegisterUncreatableType<MainViewModel>("de.skycoder42.anirem", 1, 0, "MainViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<AddAnimeViewModel>("de.skycoder42.anirem", 1, 0, "AddAnimeViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<DetailsViewModel>("de.skycoder42.anirem", 1, 0, "DetailsViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<EntryViewModel>("de.skycoder42.anirem", 1, 0, "EntryViewModel", QStringLiteral("ViewModels cannot be created!"));
	qmlRegisterUncreatableType<LoginViewModel>("de.skycoder42.anirem", 1, 0, "LoginViewModel", QStringLiteral("ViewModels cannot be created!"));

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/qml/App.qml")));
	engine.addImageProvider(QStringLiteral("proxer"), new ProxerImageProvider());
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
