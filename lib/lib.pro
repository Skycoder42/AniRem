TEMPLATE = lib

QT = core datasync

TARGET = anirem

DEFINES += BUILD_LIB_ANIREM

HEADERS += lib_anirem_global.h \
	libanirem.h

SOURCES += \
	libanirem.cpp
