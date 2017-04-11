#include "cachingnamfactory.h"
#include <cachingnam.h>

CachingNamFactory::CachingNamFactory() {}

QNetworkAccessManager *CachingNamFactory::create(QObject *parent)
{
	return new CachingNam(parent);
}
