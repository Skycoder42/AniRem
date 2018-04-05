TEMPLATE = subdirs

SUBDIRS += \
	core \
	widgets \
	quick

widgets.depends += core
quick.depends += core

prepareRecursiveTarget(lrelease)
QMAKE_EXTRA_TARGETS += lrelease
