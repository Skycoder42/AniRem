TEMPLATE = app

QT += widgets mvvmwidgets

TARGET = anirem

HEADERS += mainwindow.h

SOURCES += main.cpp \
	mainwindow.cpp

FORMS += mainwindow.ui

TRANSLATIONS += gui_widgets_de.ts \
	gui_widgets_template.ts

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
