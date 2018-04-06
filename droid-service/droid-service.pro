TEMPLATE = app

QT += androidextras
QT -= gui

TARGET = anirem-service

HEADERS +=

SOURCES += \
		main.cpp

# TODO debug
QMAKE_EXTRA_TARGETS += lrelease

include(../lib.pri)
