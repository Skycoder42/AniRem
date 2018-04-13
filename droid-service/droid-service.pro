TEMPLATE = app

QT += androidextras mvvmcore
QT -= gui

TARGET = anirem-service

HEADERS += \
	starthelper.h \
	androidupdatenotifier.h

SOURCES += \
	main.cpp \
	starthelper.cpp \
	androidupdatenotifier.cpp

QMAKE_EXTRA_TARGETS += lrelease qtifw

include(../lib.pri)
