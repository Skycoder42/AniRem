#include "cachingnamfactory.h"
#include "core.h"

CachingNamFactory::CachingNamFactory() {}

QNetworkAccessManager *CachingNamFactory::create(QObject *parent)
{
	return Core::createImageLoaderNam(parent);
}
