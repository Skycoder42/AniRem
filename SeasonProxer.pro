#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T21:01:28
#
#-------------------------------------------------

QT       += core gui widgets network

TEMPLATE = app

TARGET = SeasonProxer
VERSION = 1.0.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""

RC_ICONS += ./resources/main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

HEADERS += \
    mainwindow.h \
    app.h \
    animeseasonmodel.h \
    animeinfo.h \
    addanimedialog.h

SOURCES += \
    mainwindow.cpp \
    app.cpp \
    animeseasonmodel.cpp \
    animeinfo.cpp \
    addanimedialog.cpp

FORMS += \
	mainwindow.ui \
    addanimedialog.ui

RESOURCES += \
    seasonproxer_res.qrc
