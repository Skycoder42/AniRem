# link against main lib
QT += datasync restclient

ARLIB_DIR = $$shadowed($$PWD/lib)
win32:CONFIG(release, debug|release): LIBS += -L$$ARLIB_DIR/release/ -lanirem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$ARLIB_DIR/debug/ -lanirem
else:unix: LIBS += -L$$ARLIB_DIR/ -lanirem

INCLUDEPATH += $$PWD/lib $$ARLIB_DIR
DEPENDPATH += $$PWD/lib
