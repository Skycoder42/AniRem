#ifndef LIBANIREM_H
#define LIBANIREM_H

#include <QtDataSync/Setup>
#include <QtDataSync/DataTypeStore>

#include "lib_anirem_global.h"
#include "animeinfo.h"
#include "localsettings.h"

using AniremStore = QtDataSync::DataTypeStore<AnimeInfo, int>;

namespace AniRem {

LIB_ANIREM_EXPORT void prepareTranslations();
LIB_ANIREM_EXPORT void setup(QtDataSync::Setup &setup, bool passive = false);
LIB_ANIREM_EXPORT void setProxerToken(const QString &token, LocalSettings *settings);

}

#endif // LIBANIREM_H
