TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += Core
!android: SUBDIRS += Desktop
SUBDIRS += Mobile

qpmlrelease.target = lrelease
qpmlrelease.CONFIG += recursive
qpmlrelease.recurse_target = lrelease
QMAKE_EXTRA_TARGETS += qpmlrelease
