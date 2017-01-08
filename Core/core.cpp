#include "core.h"
#include <QCoreApplication>

#include "animeinfo.h"

static void startup();
Q_COREAPP_STARTUP_FUNCTION(startup)

static void startup()
{
	qRegisterMetaType<AnimePtr>();
	qRegisterMetaType<AnimeList>();
}
