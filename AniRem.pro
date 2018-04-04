TEMPLATE = subdirs

SUBDIRS += \
	gui \
	lib

gui.depends += lib

DISTFILES += .qmake.conf
