TEMPLATE = subdirs

SUBDIRS += \
	core \
	widgets \
	quick

widgets.depends += core
quick.depends += core

android: SUBDIRS -= widgets
no_quick: SUBDIRS -= quick

prepareRecursiveTarget(lrelease)
prepareRecursiveTarget(deploy)
QMAKE_EXTRA_TARGETS += lrelease deploy
