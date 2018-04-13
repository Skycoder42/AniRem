TEMPLATE = subdirs

SUBDIRS += \
	gui \
	lib

gui.depends += lib

android {
	SUBDIRS += droid-service
	droid-service.depends += lib
}

prepareRecursiveTarget(lrelease)
prepareRecursiveTarget(qtifw)
qtifw.depends += install
QMAKE_EXTRA_TARGETS += lrelease qtifw

DISTFILES += .qmake.conf
