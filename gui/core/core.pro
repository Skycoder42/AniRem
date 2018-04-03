TEMPLATE = lib

QT += mvvmcore
CONFIG += static

TARGET = anirem-core

HEADERS += \
	mainviewmodel.h \
	aniremapp.h

SOURCES += \
	mainviewmodel.cpp \
	aniremapp.cpp

RESOURCES += \
	anirem-core.qrc

TRANSLATIONS += gui_core_de.ts \
	gui_core_template.ts

DISTFILES += $$TRANSLATIONS
QTMVVM_TS_SETTINGS = settings.xml
_never_true_condition: SOURCES += $$files($$PWD/.ts-dummy/*)
PRE_TARGETDEPS += qtmvvm-tsgen
