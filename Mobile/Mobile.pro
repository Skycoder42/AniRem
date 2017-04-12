TEMPLATE = app

QT += qml quick network svg datasync restclient
CONFIG += c++11

VERSION = 1.0.0

RC_ICONS += ../Core/main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

QPM_INCLUDEPATH = $$PWD/../Core/vendor/vendor.pri
include(vendor/vendor.pri)

HEADERS += \
	cachingnamfactory.h \
	notifyingpresenter.h

SOURCES += main.cpp \
	cachingnamfactory.cpp \
	notifyingpresenter.cpp

RESOURCES += \
	seasonproxer_mobile.qrc

DISTFILES += \
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
	android/res/drawable-hdpi/ic_notification.png \
	android/res/drawable-mdpi/ic_notification.png \
	android/res/drawable-xhdpi/ic_notification.png \
	android/res/drawable-xxhdpi/ic_notification.png \
	android/res/drawable-xxxhdpi/ic_notification.png

android {
	HEADERS += statusview.h
	SOURCES += statusview.cpp
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lSeasonProxerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lSeasonProxerCore
else:mac: LIBS += -F$$OUT_PWD/../Core/ -framework SeasonProxerCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lSeasonProxerCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/libSeasonProxerCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/libSeasonProxerCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/SeasonProxerCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/SeasonProxerCore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Core/libSeasonProxerCore.a
