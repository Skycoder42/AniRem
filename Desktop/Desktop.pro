#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T13:06:33
#
#-------------------------------------------------
TEMPLATE = app

QT       += core gui widgets network
CONFIG += c++11

TARGET = SeasonProxer
VERSION = 1.0.0

RC_ICONS += ../Core/main.ico
QMAKE_TARGET_COMPANY = "Skycoder42"
QMAKE_TARGET_PRODUCT = "Proxer.me Season Reminder"
QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
QMAKE_TARGET_COPYRIGHT = "Felix Barz"

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

include(../QtMvvm/widgets/qtmvvmwidgets.pri)
include(../QtUtils/DialogMaster/dialogmaster.pri)

HEADERS  += mainwindow.h \
	animemodel.h \
	addanimedialog.h \
	imageloader.h \
    systemtraypresenter.h \
    statusview.h

SOURCES += \
	mainwindow.cpp \
	animemodel.cpp \
	addanimedialog.cpp \
	imageloader.cpp \
	main.cpp \
    systemtraypresenter.cpp \
    statusview.cpp

FORMS    += mainwindow.ui \
	addanimedialog.ui

RESOURCES += \
    seasonproxer_desktop.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lSeasonProxerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lSeasonProxerCore
else:mac: LIBS += -F$$OUT_PWD/../Core/ -framework SeasonProxerCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lSeasonProxerCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

win32 {
	msvc {
		contains(QT_ARCH, x86_64) {
			CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc64/release/
			else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc64/debug/
		} else {
			CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc/release/
			else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/msvc/debug/
		}
	} else:win32-g++ {
		CONFIG(release, debug|release): LIBS += -L$$PWD/../QtRestClient/mingw/release/
		else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtRestClient/mingw/debug/
	}
}
