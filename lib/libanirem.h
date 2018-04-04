#ifndef LIBANIREM_H
#define LIBANIREM_H

#include <QtDataSync/Setup>
#include <QtDataSync/DataTypeStore>

#include "lib_anirem_global.h"
#include "animeinfo.h"

using AniremStore = QtDataSync::DataTypeStore<AnimeInfo, int>;

namespace AniRem {

LIB_ANIREM_EXPORT void prepareTranslations();
LIB_ANIREM_EXPORT void setup(QtDataSync::Setup &setup, bool passive = false);

}

#endif // LIBANIREM_H
