#ifndef APIKEYS_H
#define APIKEYS_H

#ifdef BUILD_LIB_ANIREM
#include "../../__private/proxer-api-key.h"
#ifndef PROXER_API_KEY
#error You have to modify this file to contain your Proxer-Api-Key!
#endif
#endif

#endif // APIKEYS_H
