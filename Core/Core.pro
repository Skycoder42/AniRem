#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:05:30
#
#-------------------------------------------------
TEMPLATE = lib

QT       += network
QT       -= gui

TARGET = Core

DEFINES += CORE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

include(../QtRestClient/qtrestclient.pri)

HEADERS += core.h\
        core_global.h

SOURCES += core.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
