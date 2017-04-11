#ifndef APIKEYS_H
#define APIKEYS_H

#include "../../__private/proxer-api-key.h"
#include "../../__private/proxer-datasync-secret.h"

#ifndef PROXER_API_KEY
#error You have to modify this file to contain your Proxer-Api-Key!
#endif
#ifndef DATASYNC_SERVER_SECRET
#error You have to modify this file to contain your datasync server secret
#endif

#endif // APIKEYS_H
