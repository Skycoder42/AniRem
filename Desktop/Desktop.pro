#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:06:33
#
#-------------------------------------------------
TEMPLATE = app

QT       += core gui widgets network

TARGET = SeasonProxer
VERSION = 1.0.0

RC_ICONS += ./resources/main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

include(../QtRestClient/qtrestclient.pri)
include(../QtMvvm/widgets/qtmvvmwidgets.pri)
include(../QtUtils/DialogMaster/dialogmaster.pri)

HEADERS  += mainwindow.h \
	animemodel.h \
	app.h \
	addanimedialog.h \
	imageloader.h

SOURCES += \
	mainwindow.cpp \
	animemodel.cpp \
	app.cpp \
	addanimedialog.cpp \
	imageloader.cpp

FORMS    += mainwindow.ui \
	addanimedialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lSeasonProxerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lSeasonProxerCore
else:mac: LIBS += -F$$OUT_PWD/../Core/ -framework SeasonProxerCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lSeasonProxerCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

RESOURCES += \
	seasonproxer_desktop.qrc
