INCLUDEPATH += $$PWD/../core $$PWD

HEADERS += \
	$$PWD/widgetpresenter.h \
	$$PWD/ipresentingwidget.h \
	$$PWD/objectproxymodel.h \
    $$PWD/settingsdialog.h \
    $$PWD/settingswidgetfactory.h

SOURCES += \
	$$PWD/widgetpresenter.cpp \
	$$PWD/objectproxymodel.cpp \
    $$PWD/settingsdialog.cpp \
    $$PWD/settingswidgetfactory.cpp

FORMS += \
    $$PWD/settingsdialog.ui

RESOURCES += \
    $$PWD/qtmvvmwidgets.qrc
