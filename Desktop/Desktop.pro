#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:06:33
#
#-------------------------------------------------
TEMPLATE = app

QT       += core gui widgets network datasync restclient
CONFIG += c++11

!no_updater:QT += autoupdatergui
else: DEFINES += NO_AUTO_UPDATER

TARGET = AniRem
VERSION = $$SP_VERSION

RC_ICONS += ../icn/anirem.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = $$TARGET
QMAKE_TARGET_DESCRIPTION = "AniRem — Anime Season Reminder"
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

ICON = ../icn/anirem.icns
QMAKE_TARGET_BUNDLE_PREFIX = de.skycoder42

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS  += mainwindow.h \
	animemodel.h \
	addanimedialog.h \
	systemtraypresenter.h \
	statusview.h \
	detailsdockwidget.h

SOURCES += \
	mainwindow.cpp \
	animemodel.cpp \
	addanimedialog.cpp \
	main.cpp \
	systemtraypresenter.cpp \
	statusview.cpp \
	detailsdockwidget.cpp

FORMS    += mainwindow.ui \
	addanimedialog.ui \
	detailsdockwidget.ui

RESOURCES += \
	seasonproxer_desktop.qrc

DISTFILES += \
	seasonproxer_desktop_de.ts \
	seasonproxer_de.ts \
	no_updater/seasonproxer_de.ts \
	config.xml \
	meta/package.xml \
	main.png

TRANSLATIONS += seasonproxer_desktop_de.ts
!no_updater: TRANSLATIONS += seasonproxer_de.ts \
	QtAutoUpdaterController_de.ts #TODO use qm from installation
else: TRANSLATIONS += no_updater/seasonproxer_de.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lAniRemCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lAniRemCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lAniRemCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/libAniRemCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/libAniRemCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/release/AniRemCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Core/debug/AniRemCore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Core/libAniRemCore.a

# deployment
CONFIG += qtifw_auto_deploy
QTIFW_DEPLOY_TSPRO = $$_PRO_FILE_ $$_PRO_FILE_PWD_/../Core/Core.pro

# installer
QTIFW_TARGET = "SeasonProxer Installer"
QTIFW_CONFIG = config.xml
QTIFW_MODE = online_all

proxerpkg.pkg = de.skycoder42.seasonproxer
proxerpkg.meta = meta
linux: proxerpkg.files = ../icn/main.png
QTIFW_AUTO_INSTALL_PKG = proxerpkg

QTIFW_PACKAGES += proxerpkg

# make install
no_updater {
	target.path = /usr/bin
	INSTALLS += target
} else:CONFIG += qtifw_install_target

# qpm
QPM_INCLUDEPATH = $$PWD/../Core/vendor/vendor.pri
include(vendor/vendor.pri)
