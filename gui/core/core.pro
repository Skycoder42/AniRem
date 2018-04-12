TEMPLATE = lib

QT += mvvmcore mvvmdatasynccore
android: QT += androidextras
CONFIG += static
win32: CONFIG += skip_target_version_ext

TARGET = anirem-core

HEADERS += \
	mainviewmodel.h \
	aniremapp.h \
	datasyncsettingsviewmodel.h \
	addanimeviewmodel.h \
	imageloader.h \
	detailsviewmodel.h \
	aniremsettingsviewmodel.h \
	entryviewmodel.h \
	proxerentrymodel.h \
	loginviewmodel.h

SOURCES += \
	mainviewmodel.cpp \
	aniremapp.cpp \
	datasyncsettingsviewmodel.cpp \
	addanimeviewmodel.cpp \
	imageloader.cpp \
	detailsviewmodel.cpp \
	aniremsettingsviewmodel.cpp \
	entryviewmodel.cpp \
	proxerentrymodel.cpp \
	loginviewmodel.cpp

RESOURCES += \
	anirem-core.qrc

TRANSLATIONS += anirem_core_de.ts

DISTFILES += $$TRANSLATIONS
QTMVVM_TS_SETTINGS = settings.xml
_never_true_condition: SOURCES += $$files($$PWD/.ts-dummy/*)
PRE_TARGETDEPS += qtmvvm-tsgen

# install
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += qpmx_ts_target

include(../../lib.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

QMAKE_EXTRA_TARGETS += deploy
