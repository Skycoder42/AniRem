#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:06:33
#
#-------------------------------------------------
TEMPLATE = app

QT       += core gui  widgets

TARGET = Desktop

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS  += mainwindow.h

SOURCES += main.cpp\
		mainwindow.cpp

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:mac: LIBS += -F$$OUT_PWD/../Core/ -framework Core
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core
