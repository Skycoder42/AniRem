TEMPLATE = app

QT += quick mvvmquick mvvmdatasyncquick

TARGET = anirem

HEADERS += \
	proxerimageprovider.h

SOURCES += main.cpp \
	proxerimageprovider.cpp

RESOURCES += \
	anirem-quick.qrc

TRANSLATIONS += anirem_quick_de.ts

EXTRA_TRANSLATIONS += anirem_de.ts

DISTFILES += $$TRANSLATIONS \
	$$EXTRA_TRANSLATIONS

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

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
