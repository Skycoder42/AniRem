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
prepareRecursiveTarget(deploy)
deploy.depends += install
QMAKE_EXTRA_TARGETS += lrelease deploy

DISTFILES += .qmake.conf
