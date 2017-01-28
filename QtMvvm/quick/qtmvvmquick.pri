INCLUDEPATH += $$PWD/../core $$PWD

HEADERS += \
    $$PWD/quickpresenter.h \
    $$PWD/settingsuibuilder.h \
    $$PWD/settingsoverelement.h

SOURCES += \
    $$PWD/quickpresenter.cpp \
    $$PWD/settingsuibuilder.cpp \
    $$PWD/settingsoverelement.cpp

RESOURCES += \
    $$PWD/qtmvvmquick.qrc

include(../QuickExtras/quickextras.pri)
