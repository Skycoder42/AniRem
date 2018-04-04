TEMPLATE = lib

QT = core datasync restclient mvvmcore

TARGET = anirem

DEFINES += BUILD_LIB_ANIREM

HEADERS += lib_anirem_global.h \
	libanirem.h \
	api/apihelper.h \
	apikeys.h \
	animeinfo.h \
	jsonseasondataconverter.h

SOURCES += \
	libanirem.cpp \
	api/apihelper.cpp \
	animeinfo.cpp \
	jsonseasondataconverter.cpp

REST_API_OBJECTS += api/proxerstatus.json \
	api/proxerentryvalue.json \
	api/proxerentry.json \
	api/proxerrelations.json

REST_API_CLASSES += api/proxerapi.json \
	api/infoclass.json

SETTINGS_GENERATORS += \
	localsettings.xml \
	syncedsettings.xml

MVVM_SETTINGS_FILES += $$PWD/../gui/core/settings.xml

TRANSLATIONS += anirem_lib_de.ts \
	anirem_lib_template.ts

DISTFILES += $$TRANSLATIONS

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

# install copy
for(header, SETTINGSGENERATOR_BUILD_HEADERS) {
	theader = $$shadowed($$SETTINGSGENERATOR_DIR/$$basename(header))
	!exists($$theader):system($$QMAKE_COPY_FILE $$shell_quote($$shell_path($$header)) $$shell_quote($$shell_path($$theader)))
}
