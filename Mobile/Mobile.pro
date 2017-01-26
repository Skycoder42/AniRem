TEMPLATE = app

QT += qml quick network svg
CONFIG += c++11

TARGET = SeasonProxer
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

include(../QtMvvm/quick/qtmvvmquick.pri)

HEADERS += \
	cachingnamfactory.h \
    notifyingpresenter.h

SOURCES += main.cpp \
	cachingnamfactory.cpp \
    notifyingpresenter.cpp

RESOURCES += qml.qrc

DISTFILES += \
	android/AndroidManifest.xml \
	android/res/values/libs.xml \
	android/build.gradle \
	android/src/de/skycoder42/seasonproxer/Notifier.java \
    android/res/drawable-hdpi/ic_launcher.png \
    android/res/drawable-mdpi/ic_launcher.png \
    android/res/drawable-xhdpi/ic_launcher.png \
    android/res/drawable-xxhdpi/ic_launcher.png \
    android/res/drawable-xxxhdpi/ic_launcher.png \
    android/res/values/strings.xml

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

win32 {
	msvc {
		contains(QT_ARCH, x86_64) {
			CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc64/release/
			else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc64/debug/
		} else {
			CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc/release/
			else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc/debug/
		}
	} else:win32-g++ {
		CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/mingw/release/
		else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/mingw/debug/
	}
} else:android:contains(QT_ARCH, arm) {
	# extra dependencies from core
	ANDROID_EXTRA_LIBS += \
		$$[QT_INSTALL_LIBS]/libQt5Concurrent.so# \
#		$$[QT_INSTALL_LIBS]/libQt5Sql.so
	QT += sql #required like this because of the sqlite plugin
	CONFIG(release, debug|release): ANDROID_EXTRA_LIBS += $$PWD/../QtRestClient/android_armv7/release/libQtRestClient.so
	CONFIG(debug, debug|release): ANDROID_EXTRA_LIBS += $$PWD/../QtRestClient/android_armv7/debug/libQtRestClient.so
}
