#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:05:30
#
#-------------------------------------------------
TEMPLATE = lib

QT       += core gui network concurrent sql
android: QT += androidextras
win32: CONFIG += skip_target_version_ext

TARGET = SeasonProxerCore
VERSION = 1.0.0

RC_ICONS += ./main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder core library"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += CORE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

include(../QtRestClient/qtrestclient.pri)
include(../QtMvvm/core/qtmvvmcore.pri)

HEADERS += core.h\
		core_global.h \
	animestore.h \
	animeinfo.h \
	countlock.h \
	proxer-api-key.h \
	ProxerApi/proxerentry.h \
	ProxerApi/proxerrelations.h \
	ProxerApi/proxerstatus.h \
	ProxerApi/infoclass.h \
	proxerapp.h \
	maincontrol.h \
    addanimecontrol.h \
	seasonstatusloader.h \
    statuscontrol.h

SOURCES += core.cpp \
	animestore.cpp \
	animeinfo.cpp \
	countlock.cpp \
	ProxerApi/proxerentry.cpp \
	ProxerApi/proxerrelations.cpp \
	ProxerApi/proxerstatus.cpp \
	ProxerApi/infoclass.cpp \
	proxerapp.cpp \
	maincontrol.cpp \
    addanimecontrol.cpp \
	seasonstatusloader.cpp \
	statuscontrol.cpp

RESOURCES += \
	seasonproxer_core.qrc
