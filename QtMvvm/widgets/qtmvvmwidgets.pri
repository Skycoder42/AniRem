INCLUDEPATH += $$PWD/../core $$PWD

HEADERS += \
	$$PWD/widgetpresenter.h \
	$$PWD/ipresentingwidget.h \
	$$PWD/objectproxymodel.h \
    $$PWD/settingsdialog.h \
    $$PWD/inputwidgetfactory.h

SOURCES += \
	$$PWD/widgetpresenter.cpp \
	$$PWD/objectproxymodel.cpp \
    $$PWD/settingsdialog.cpp \
    $$PWD/inputwidgetfactory.cpp

FORMS += \
    $$PWD/settingsdialog.ui

RESOURCES += \
    $$PWD/qtmvvmwidgets.qrc
