#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:05:30
#
#-------------------------------------------------
TEMPLATE = lib

load(qrestbuilder)

QT += core gui network datasync restclient jsonserializer
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

include(../QtMvvm/core/qtmvvmcore.pri)

HEADERS += core.h\
		core_global.h \
	animestore.h \
	animeinfo.h \
	proxer-api-key.h \
	proxerapp.h \
	maincontrol.h \
	addanimecontrol.h \
	seasonstatusloader.h \
	statuscontrol.h \
	proxersettingscontrol.h \
	detailscontrol.h \
	jsonserializer.h \
	ProxerApi/equals_fix.h \
    ProxerApi/apihelper.h

SOURCES += core.cpp \
	animestore.cpp \
	animeinfo.cpp \
	proxerapp.cpp \
	maincontrol.cpp \
	addanimecontrol.cpp \
	seasonstatusloader.cpp \
	statuscontrol.cpp \
	proxersettingscontrol.cpp \
	detailscontrol.cpp \
	jsonserializer.cpp \
    ProxerApi/apihelper.cpp

REST_API_OBJECTS += ProxerApi/proxerstatus.json \
	ProxerApi/proxerentryvalue.json \
	ProxerApi/proxerentry.json \
	ProxerApi/proxerrelations.json

REST_API_CLASSES += ProxerApi/proxerapi.json \
	ProxerApi/infoclass.json

RESOURCES += \
	seasonproxer_core.qrc

DISTFILES += \
	ProxerApi/proxerapi_kopie.json
