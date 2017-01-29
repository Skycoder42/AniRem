INCLUDEPATH += $$PWD/../core $$PWD

HEADERS += \
    $$PWD/quickpresenter.h \
    $$PWD/settingsuibuilder.h \
    $$PWD/settingsoverelement.h \
    $$PWD/settingsentryelement.h \
    $$PWD/inputviewfactory.h \
    $$PWD/multifilterproxymodel.h

SOURCES += \
    $$PWD/quickpresenter.cpp \
    $$PWD/settingsuibuilder.cpp \
    $$PWD/settingsoverelement.cpp \
    $$PWD/settingsentryelement.cpp \
    $$PWD/inputviewfactory.cpp \
    $$PWD/multifilterproxymodel.cpp

RESOURCES += \
    $$PWD/qtmvvmquick.qrc

include(../QuickExtras/quickextras.pri)
