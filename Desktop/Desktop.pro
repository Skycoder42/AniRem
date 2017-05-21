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

TARGET = SeasonProxer
VERSION = 1.0.0

RC_ICONS += ../Core/main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

ICON = main.icns
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
	deploy_x11.sh \
	deploy_win.bat \
	deploy_mac.command \
	config.xml \
	meta/package.xml \
	main.png

TRANSLATIONS += seasonproxer_desktop_de.ts
!no_updater: TRANSLATIONS += seasonproxer_de.ts
else: TRANSLATIONS += no_updater/seasonproxer_de.ts

!no_updater:TRANSLATIONS += QtAutoUpdaterController_de.ts #TODO use qm from installation

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

# deployment
deploy.target = deploy
linux: deploy.commands = $$shell_quote($$PWD/deploy_x11.sh) $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$[QT_INSTALL_LIBS]) $$shell_quote($$[QT_INSTALL_PLUGINS]) $$shell_quote($$[QT_INSTALL_TRANSLATIONS]) $$shell_quote($$_PRO_FILE_PWD_/..)
else:win32:CONFIG(release, debug|release): deploy.commands = $$shell_quote($$PWD/deploy_win.bat) release $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$[QT_INSTALL_TRANSLATIONS]) $$shell_quote($$_PRO_FILE_PWD_/..)
else:win32:CONFIG(debug, debug|release): deploy.commands = $$shell_quote($$PWD/deploy_win.bat) debug $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$[QT_INSTALL_TRANSLATIONS]) $$shell_quote($$_PRO_FILE_PWD_/..)
else:mac: deploy.commands = $$shell_quote($$PWD/deploy_mac.command) $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$[QT_INSTALL_TRANSLATIONS]) $$shell_quote($$_PRO_FILE_PWD_/..)
QMAKE_EXTRA_TARGETS += deploy

# installer
QTIFW_TARGET = "SeasonProxer Installer"
QTIFW_CONFIG = config.xml
QTIFW_MODE = online_all

proxerpkg.pkg = de.skycoder42.seasonproxer
proxerpkg.meta = meta
!mac: proxerpkg.data = $$OUT_PWD/deployment
else: proxerpkg.data = $$OUT_PWD/deployment/SeasonProxer.app

QTIFW_PACKAGES += proxerpkg

# qpm
QPM_INCLUDEPATH = $$PWD/../Core/vendor/vendor.pri
include(vendor/vendor.pri)

# make install
no_updater {
	target.path = /usr/bin
	INSTALLS += target
} else {
	target_d.target = target_d
	target_d.commands = echo deploying
	target_d.depends = deploy

	target_i.target = target_i
	target_i.commands = echo creating installer
	target_i.depends = target_d installer

	target_r.target = install
	target_r.commands = $$QMAKE_COPY_DIR $$OUT_PWD/qtifw-installer $(INSTALL_ROOT)
	target_r.depends = target_i

	QMAKE_EXTRA_TARGETS += target_d target_i target_r
}
