#ifndef LIBANIREM_H
#define LIBANIREM_H

#include <QtDataSync/Setup>

#include "lib_anirem_global.h"

namespace AniRem {

LIB_ANIREM_EXPORT void prepareTranslations();
LIB_ANIREM_EXPORT void setup(QtDataSync::Setup &setup, bool passive = false);

}

#endif // LIBANIREM_H
