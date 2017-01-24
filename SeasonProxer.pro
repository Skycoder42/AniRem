TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += Core
!android: SUBDIRS += Desktop
SUBDIRS += Mobile
