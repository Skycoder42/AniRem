TEMPLATE = app

QT += qml quick network svg datasync restclient
CONFIG += c++11

TARGET = SeasonProxer
VERSION = $$SP_VERSION 

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"Proxer.me Season Reminder\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

QPM_INCLUDEPATH = $$PWD/../Core/vendor/vendor.pri
include(vendor/vendor.pri)

HEADERS += \
	notifyingpresenter.h \
	proxerimageprovider.h

SOURCES += main.cpp \
	notifyingpresenter.cpp \
	proxerimageprovider.cpp

RESOURCES += seasonproxer_mobile.qrc
android: RESOURCES += seasonproxer_android.qrc

DISTFILES += \
	seasonproxer_mobile_de.ts \
	seasonproxer_de.ts \
	android/AndroidManifest.xml \
	android/res/values/libs.xml \
	android/build.gradle \
	android/res/drawable-hdpi/ic_launcher.png \
	android/res/drawable-mdpi/ic_launcher.png \
	android/res/drawable-xhdpi/ic_launcher.png \
	android/res/drawable-xxhdpi/ic_launcher.png \
	android/res/drawable-xxxhdpi/ic_launcher.png \
	android/res/values/strings.xml \
	android/src/de/skycoder42/seasonproxer/SeasonProxerService.java \
	android/src/de/skycoder42/seasonproxer/AlarmReceiver.java \
	android/res/values/styles.xml \ 
	android/res/drawable-hdpi/ic_notification.png \ 
	android/res/drawable-hdpi/splash.9.png \ 
	android/res/drawable-mdpi/ic_notification.png \ 
	android/res/drawable-mdpi/splash.9.png \ 
	android/res/drawable-xhdpi/ic_notification.png \ 
	android/res/drawable-xhdpi/splash.9.png \ 
	android/res/drawable-xxhdpi/ic_notification.png \ 
	android/res/drawable-xxhdpi/splash.9.png \ 
	android/res/drawable-xxxhdpi/ic_notification.png \ 
	android/res/drawable-xxxhdpi/splash.9.png \ 
	android/res/drawable-hdpi/ic_notification.png \
	android/res/drawable-mdpi/ic_notification.png \
	android/res/drawable-xhdpi/ic_notification.png \
	android/res/drawable-xxhdpi/ic_notification.png \
	android/res/drawable-xxxhdpi/ic_notification.png \
	android/res/values/styles.xml \
	android/res/values-de/strings.xml

TRANSLATIONS += seasonproxer_mobile_de.ts \
	seasonproxer_de.ts

android {
	HEADERS += statusview.h
	SOURCES += statusview.cpp
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lSeasonProxerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lSeasonProxerCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lSeasonProxerCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/libSeasonProxerCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/libSeasonProxerCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/SeasonProxerCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/SeasonProxerCore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Core/libSeasonProxerCore.a

# translation install
qtPrepareTool(LRELEASE, lrelease)
LRELEASE += -compress -nounfinished

lrelease_target.target = lrelease_core
lrelease_target.commands = $$LRELEASE $$PWD/../Core/Core.pro && $$LRELEASE $$_PRO_FILE_

ts_target.files = "$$[QT_INSTALL_TRANSLATIONS]/qtbase_*.qm" "$$[QT_INSTALL_TRANSLATIONS]/qtwebsockets_*.qm" "$$PWD/../Core/*.qm" "$$PWD/*.qm"
ts_target.path = /assets/translations
ts_target.depends += lrelease_target

QMAKE_EXTRA_TARGETS += lrelease_target
INSTALLS += ts_target
