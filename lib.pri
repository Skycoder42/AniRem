# link against main lib
QT += datasync restclient mvvmcore

ARLIB_DIR = $$shadowed($$PWD/lib)
win32:CONFIG(release, debug|release): LIBS += -L$$ARLIB_DIR/release/ -lanirem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$ARLIB_DIR/debug/ -lanirem
else:mac: LIBS += -F$$ARLIB_DIR/ -framework anirem
else: LIBS += -L$$ARLIB_DIR/ -lanirem

INCLUDEPATH += $$PWD/lib $$ARLIB_DIR
win32:CONFIG(release, debug|release): INCLUDEPATH += $$ARLIB_DIR/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$ARLIB_DIR/debug
DEPENDPATH += $$PWD/lib
