#ifndef EQUALS_FIX_H
#define EQUALS_FIX_H

#include "core_global.h"
#include "proxerentryvalue.h"

CORESHARED_EXPORT inline bool operator ==(const ProxerEntryValue &a, const ProxerEntryValue &b)
{
	return a.id() == b.id();
}

#endif // EQUALS_FIX_H
