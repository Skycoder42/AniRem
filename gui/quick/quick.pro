TEMPLATE = app

QT += quick mvvmquick mvvmdatasyncquick concurrent
android: QT += androidextras

android: TARGET = anirem-activity
else: TARGET = anirem

HEADERS += \
	proxerimageprovider.h \
	qmltoast.h

SOURCES += main.cpp \
	proxerimageprovider.cpp \
	qmltoast.cpp

RESOURCES += \
	anirem-quick.qrc

TRANSLATIONS += anirem_quick_de.ts

EXTRA_TRANSLATIONS += anirem_de.ts

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += $$TRANSLATIONS \
	$$EXTRA_TRANSLATIONS \
	android/AndroidManifest.xml \
	android/build.gradle \
	android/src/de/skycoder42/anirem/* \
	android/res/values/* \
	android/res/values-de/* \
	android/res/xml/* \
	android/res/drawable-hdpi/* \
	android/res/drawable-mdpi/* \
	android/res/drawable-xhdpi/* \
	android/res/drawable-xxhdpi/* \
	android/res/drawable-xxxhdpi/* \
	android/res/mipmap-hdpi/* \
	android/res/mipmap-mdpi/* \
	android/res/mipmap-xhdpi/* \
	android/res/mipmap-xxhdpi/* \
	android/res/mipmap-xxxhdpi/* \
	android/res/mipmap-anydpi-v26/* \
	android/src/de/skycoder42/anirem/Globals.java

# install
target.path = $$INSTALL_BINS
qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
extra_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += target qpmx_ts_target extra_ts_target

# Link with core project
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lanirem-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lanirem-core
else:unix: LIBS += -L$$OUT_PWD/../core/ -lanirem-core

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libanirem-core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libanirem-core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/anirem-core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/anirem-core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libanirem-core.a

include(../../lib.pri)

android {
	LIBS += -L$$PWD/openssl/openssl -lcrypto -lssl
	ANDROID_EXTRA_LIBS += \
		$$PWD/openssl/openssl/libcrypto.so \
		$$PWD/openssl/openssl/libssl.so
	RESOURCES += anirem-android.qrc

	tsqtInstall.path = $$INSTALL_TRANSLATIONS
	tsqtInstall.files = \
		$$[QT_INSTALL_TRANSLATIONS]/qtbase_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtdeclarative_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtquickcontrols_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtwebsockets_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtdatasync_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtmvvmcore_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtmvvmdatasynccore_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtmvvmquick_*.qm \
		$$[QT_INSTALL_TRANSLATIONS]/qtmvvmdatasyncquick_*.qm
	INSTALLS += tsqtInstall
}

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)

QMAKE_EXTRA_TARGETS += qtifw
