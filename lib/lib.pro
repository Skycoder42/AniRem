TEMPLATE = lib

QT = core datasync restclient mvvmcore
win32: CONFIG += skip_target_version_ext
CONFIG += lib_bundle

TARGET = anirem

DEFINES += BUILD_LIB_ANIREM

PUBLIC_HEADERS = \
	$$PWD/lib_anirem_global.h \
	$$PWD/libanirem.h \
	$$PWD/apihelper.h \
	$$PWD/seasonstatusloader.h \
	$$PWD/iupdatenotifier.h \
	$$PWD/animeinfo.h \
	$$PWD/passiveupdater.h

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

win32:CONFIG(release, debug|release): OUT_INC_DIR += $$OUT_PWD/release
else:win32:CONFIG(debug, debug|release): OUT_INC_DIR += $$OUT_PWD/debug
else: OUT_INC_DIR = $$OUT_PWD
INCLUDEPATH += $$OUT_INC_DIR

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
	$$OUT_INC_DIR/localsettings.h \
	$$OUT_INC_DIR/syncedsettings.h \
	$$OUT_INC_DIR/proxerstatus.h \
	$$OUT_INC_DIR/proxerentryvalue.h \
	$$OUT_INC_DIR/proxerentry.h \
	$$OUT_INC_DIR/proxerrelations.h \
	$$OUT_INC_DIR/infoclass.h \
	$$OUT_INC_DIR/proxerapi.h
header_install.CONFIG += no_check_exist
header_install.path = $$INSTALL_HEADERS/anirem
INSTALLS += target qpmx_ts_target
!android:!mac: INSTALLS += header_install

# install osx headers
FRAMEWORK_HEADERS.version = Versions
for(hfile, header_install.files): FRAMEWORK_HEADERS.files += $$relative_path($$hfile, $$OUT_PWD)
message($${FRAMEWORK_HEADERS.files})
FRAMEWORK_HEADERS.path = Headers
QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS

QMAKE_EXTRA_TARGETS += qtifw
