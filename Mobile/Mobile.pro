TEMPLATE = app

QT += qml quick network concurrent sql svg
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
include(../QtAndroidStuff/qtandroidstuff.pri)

SOURCES += main.cpp \
	cachingnamfactory.cpp

RESOURCES += qml.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lSeasonProxerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lSeasonProxerCore
else:mac: LIBS += -F$$OUT_PWD/../Core/ -framework SeasonProxerCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lSeasonProxerCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

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
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	CONFIG(release, debug|release): ANDROID_EXTRA_LIBS = $$PWD/../QtRestClient/android_armv7/release/libQtRestClient.so
	CONFIG(debug, debug|release): ANDROID_EXTRA_LIBS = $$PWD/../QtRestClient/android_armv7/debug/libQtRestClient.so
}

HEADERS += \
	cachingnamfactory.h
