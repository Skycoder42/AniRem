#include "proxerentry.h"

ProxerEntry::ProxerEntry(QObject *parent) :
	ProxerStatus(parent),
	data(nullptr)
{}

ProxerEntryData::ProxerEntryData(QObject *parent) :
	RestObject(parent),
	id(0),
	name(),
	medium(),
	year(0),
	season(0)
{}
