TEMPLATE = lib

QT = core datasync restclient mvvmcore
win32: CONFIG += skip_target_version_ext

TARGET = anirem

DEFINES += BUILD_LIB_ANIREM

PUBLIC_HEADERS = \
	lib_anirem_global.h \
	libanirem.h \
	apihelper.h \
	seasonstatusloader.h \
	iupdatenotifier.h \
	animeinfo.h \
	passiveupdater.h

HEADERS += $$PUBLIC_HEADERS \
	jsonseasondataconverter.h \
	storedcookiejar.h

SOURCES += \
	libanirem.cpp \
	apihelper.cpp \
	animeinfo.cpp \
	jsonseasondataconverter.cpp \
	seasonstatusloader.cpp \
	passiveupdater.cpp \
	storedcookiejar.cpp

REST_API_OBJECTS += api/proxerstatus.json \
	api/proxerentryvalue.json \
	api/proxerentry.json \
	api/proxerrelations.json \
	api/proxernamevalue.json \
	api/proxernames.json \
	api/proxerlistvalue.json \
	api/proxerlist.json \
	api/proxerlogin.json \
	api/proxerloginvalue.json

REST_API_CLASSES += api/proxerapi.json \
	api/infoclass.json \
	api/userclass.json

SETTINGS_GENERATORS += \
	localsettings.xml \
	syncedsettings.xml

MVVM_SETTINGS_FILES += $$PWD/../gui/core/settings.xml

TRANSLATIONS += anirem_lib_de.ts

DISTFILES += $$TRANSLATIONS

win32:CONFIG(release, debug|release): INCLUDEPATH += $$OUT_PWD/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$OUT_PWD/debug


!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

# install copy
system($$QMAKE_MKDIR $$shell_quote($$shell_path($$shadowed($$SETTINGSGENERATOR_DIR))))
for(header, SETTINGSGENERATOR_BUILD_HEADERS) {
	theader = $$shadowed($$SETTINGSGENERATOR_DIR/$$basename(header))
	!exists($$theader):system($$QMAKE_COPY_FILE $$shell_quote($$shell_path($$header)) $$shell_quote($$shell_path($$theader)))
}

# install
target.path = $$INSTALL_LIBS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
header_install.files = $$PUBLIC_HEADERS \
	$$SETTINGSGENERATOR_BUILD_HEADERS \
	$$SETTINGSGENERATOR_DIR/localsettings.h \
	$$SETTINGSGENERATOR_DIR/syncedsettings.h \
	$$OUT_PWD/proxerstatus.h \
	$$OUT_PWD/proxerentryvalue.h \
	$$OUT_PWD/proxerentry.h \
	$$OUT_PWD/proxerrelations.h \
	$$OUT_PWD/infoclass.h \
	$$OUT_PWD/proxerapi.h
header_install.CONFIG += no_check_exist
header_install.path = $$INSTALL_HEADERS/anirem
INSTALLS += target qpmx_ts_target
!android: INSTALLS += header_install

QMAKE_EXTRA_TARGETS += qtifw
