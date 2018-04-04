TEMPLATE = lib

QT += mvvmcore mvvmdatasynccore
CONFIG += static

TARGET = anirem-core

HEADERS += \
	mainviewmodel.h \
	aniremapp.h \
    datasyncsettingsviewmodel.h \
    addanimeviewmodel.h \
    imageloader.h \
    detailsviewmodel.h

SOURCES += \
	mainviewmodel.cpp \
	aniremapp.cpp \
    datasyncsettingsviewmodel.cpp \
    addanimeviewmodel.cpp \
    imageloader.cpp \
    detailsviewmodel.cpp

RESOURCES += \
	anirem-core.qrc

TRANSLATIONS += gui_core_de.ts \
	gui_core_template.ts

DISTFILES += $$TRANSLATIONS
QTMVVM_TS_SETTINGS = settings.xml
_never_true_condition: SOURCES += $$files($$PWD/.ts-dummy/*)
PRE_TARGETDEPS += qtmvvm-tsgen

include(../../lib.pri)
