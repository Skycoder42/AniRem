CONFIG(debug, debug|release) {
	QT += testlib
	HEADERS += ../Testing/modeltest.h
	SOURCES += ../Testing/modeltest.cpp
	INCLUDEPATH += $$PWD/../Testing
}
