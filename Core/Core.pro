#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:05:30
#
#-------------------------------------------------
TEMPLATE = lib

QT       += core gui network concurrent sql

TARGET = SeasonProxerCore

DEFINES += CORE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

include(../QtRestClient/qtrestclient.pri)

HEADERS += core.h\
        core_global.h \
    animestore.h \
    animeinfo.h \
    countlock.h \
    proxer-api-key.h \
    ProxerApi/proxerentry.h \
    ProxerApi/proxerrelations.h \
    ProxerApi/proxerstatus.h \
    ProxerApi/infoclass.h

SOURCES += core.cpp \
    animestore.cpp \
    animeinfo.cpp \
    countlock.cpp \
    ProxerApi/proxerentry.cpp \
    ProxerApi/proxerrelations.cpp \
    ProxerApi/proxerstatus.cpp \
    ProxerApi/infoclass.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    seasonproxer_core.qrc
