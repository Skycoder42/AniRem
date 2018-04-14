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

mac {
	install_join.commands += cp -pRv $$INSTALL_APPS/ $${INSTALL_PREFIX}/ || true && rm -rf $$INSTALL_APPS
	install_join.target = install
	QMAKE_EXTRA_TARGETS += install_join
}
