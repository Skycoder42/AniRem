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

HEADERS += \
	notifyingpresenter.h \
	proxerimageprovider.h

SOURCES += main.cpp \
	notifyingpresenter.cpp \
	proxerimageprovider.cpp

android {
	HEADERS += statusview.h
	SOURCES += statusview.cpp
}

RESOURCES += seasonproxer_mobile.qrc
android: RESOURCES += seasonproxer_android.qrc

DISTFILES += \
	seasonproxer_mobile_de.ts \
	seasonproxer_de.ts \
	android/AndroidManifest.xml \
	android/res/values/libs.xml \
	android/res/values/strings.xml \
	android/res/values/styles.xml \
	android/res/values-de/strings.xml \
	android/res/drawable-hdpi/ic_launcher.png \
	android/res/drawable-hdpi/ic_notification.png \
	android/res/drawable-hdpi/splash.9.png \
	android/res/drawable-mdpi/ic_launcher.png \
	android/res/drawable-mdpi/ic_notification.png \
	android/res/drawable-mdpi/splash.9.png \
	android/res/drawable-xhdpi/ic_launcher.png \
	android/res/drawable-xhdpi/ic_notification.png \
	android/res/drawable-xhdpi/splash.9.png \
	android/res/drawable-xxhdpi/ic_launcher.png \
	android/res/drawable-xxhdpi/ic_notification.png \
	android/res/drawable-xxhdpi/splash.9.png \
	android/res/drawable-xxxhdpi/ic_launcher.png \
	android/res/drawable-xxxhdpi/ic_notification.png \
	android/res/drawable-xxxhdpi/splash.9.png \
	android/build.gradle \
	android/src/de/skycoder42/seasonproxer/AlarmReceiver.java \
	android/src/de/skycoder42/seasonproxer/SeasonProxerService.java

TRANSLATIONS += seasonproxer_mobile_de.ts

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

QPM_INCLUDEPATH = $$PWD/../Core/vendor/vendor.pri
include(vendor/vendor.pri)

# custom tr
splrelease.target = lrelease-extra
splrelease.commands = $$LRELEASE $$shell_quote($$shell_path($$PWD/seasonproxer_de.ts))
qpmlcombine.depends += splrelease
QMAKE_EXTRA_TARGETS += splrelease

# translation install
tsqtInstall.path = /assets/translations
tsqtInstall.files = $$[QT_INSTALL_TRANSLATIONS]/qtbase_*.qm $$[QT_INSTALL_TRANSLATIONS]/qtwebsockets_*.qm

trInstall.path = /assets/translations
trInstall.files = $$PWD/seasonproxer_de.qm \
	$$OUT_PWD/seasonproxer_mobile_de.qm \
	$$OUT_PWD/../Core/seasonproxer_core_de.qm
trInstall.CONFIG += no_check_exist
trInstall.depends = qpmlcombine

INSTALLS += tsqtInstall trInstall
