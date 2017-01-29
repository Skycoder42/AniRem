INCLUDEPATH += $$PWD/../core $$PWD

HEADERS += \
    $$PWD/quickpresenter.h \
    $$PWD/settingsuibuilder.h \
    $$PWD/settingsoverelement.h \
    $$PWD/settingsentryelement.h \
    $$PWD/settingsdelegatefactory.h

SOURCES += \
    $$PWD/quickpresenter.cpp \
    $$PWD/settingsuibuilder.cpp \
    $$PWD/settingsoverelement.cpp \
    $$PWD/settingsentryelement.cpp \
    $$PWD/settingsdelegatefactory.cpp

RESOURCES += \
    $$PWD/qtmvvmquick.qrc

include(../QuickExtras/quickextras.pri)
