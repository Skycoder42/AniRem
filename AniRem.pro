TEMPLATE = subdirs

SUBDIRS += \
	gui \
	lib

gui.depends += lib

prepareRecursiveTarget(lrelease)
QMAKE_EXTRA_TARGETS += lrelease

DISTFILES += .qmake.conf
