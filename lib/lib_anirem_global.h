#ifndef LIB_ANIREM_GLOBAL_H
#define LIB_ANIREM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BUILD_LIB_ANIREM)
#  define LIB_ANIREM_EXPORT Q_DECL_EXPORT
#else
#  define LIB_ANIREM_EXPORT Q_DECL_IMPORT
#endif

#ifdef BUILD_LIB_ANIREM
#ifndef PROXER_API_KEY
#include "../proxer-api-key.h"
#endif
#endif

#endif // LIB_ANIREM_GLOBAL_H
