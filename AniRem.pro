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
QMAKE_EXTRA_TARGETS += lrelease qtifw

DISTFILES += .qmake.conf \
	README.md

mac {
	install_join.commands += cp -pRv "$(INSTALL_ROOT)$$INSTALL_APPS/" "$(INSTALL_ROOT)$${INSTALL_PREFIX}/" \
		$$escape_expand(\n\t)rm -rf "$(INSTALL_ROOT)$$INSTALL_APPS" \
		$$escape_expand(\n\t)install_name_tool -change anirem.framework/Versions/2/anirem @rpath/anirem.framework/Versions/2/anirem "$(INSTALL_ROOT)$${INSTALL_BINS}/Ani-Rem"
	install_join.target = install
	QMAKE_EXTRA_TARGETS += install_join
}
