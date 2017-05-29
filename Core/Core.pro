#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:05:30
#
#-------------------------------------------------
TEMPLATE = lib
CONFIG += staticlib #important because dlls are problematic

load(qrestbuilder)

QT += core gui network datasync restclient
android: QT += androidextras
win32: CONFIG += skip_target_version_ext

TARGET = AniRemCore
VERSION = $$SP_VERSION

DEFINES += QT_DEPRECATED_WARNINGS

include(vendor/vendor.pri)

HEADERS +=\
	animeinfo.h \
	proxerapp.h \
	maincontrol.h \
	addanimecontrol.h \
	seasonstatusloader.h \
	statuscontrol.h \
	proxersettingscontrol.h \
	detailscontrol.h \
	jsonserializer.h \
	ProxerApi/apihelper.h \
	apikeys.h \
	imageloader.h \
	datasyncobjectmodel.h

SOURCES += \
	animeinfo.cpp \
	proxerapp.cpp \
	maincontrol.cpp \
	addanimecontrol.cpp \
	seasonstatusloader.cpp \
	statuscontrol.cpp \
	proxersettingscontrol.cpp \
	detailscontrol.cpp \
	jsonserializer.cpp \
	ProxerApi/apihelper.cpp \
	imageloader.cpp

REST_API_OBJECTS += ProxerApi/proxerstatus.json \
	ProxerApi/proxerentryvalue.json \
	ProxerApi/proxerentry.json \
	ProxerApi/proxerrelations.json

REST_API_CLASSES += ProxerApi/proxerapi.json \
	ProxerApi/infoclass.json

RESOURCES += \
	seasonproxer_core.qrc

DISTFILES += \
	seasonproxer_core_de.ts

TRANSLATIONS += seasonproxer_core_de.ts
