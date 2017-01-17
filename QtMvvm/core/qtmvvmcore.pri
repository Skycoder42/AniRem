DEFINES += QTMVVM_CORE_LIBRARY

HEADERS += \
	$$PWD/coreapp.h \
	$$PWD/control.h \
	$$PWD/ipresenter.h \
	$$PWD/qtmvvm_core_global.h \
	$$PWD/messageresult.h \
	$$PWD/coremessage.h \
	$$PWD/objectlistmodel.h

SOURCES += \
	$$PWD/coreapp.cpp \
	$$PWD/control.cpp \
	$$PWD/messageresult.cpp \
	$$PWD/coremessage.cpp \
	$$PWD/objectlistmodel.cpp

INCLUDEPATH += $$PWD

include(../../Testing/modeltest.pri)
