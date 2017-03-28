#include "animestore.h"
#include "coremessage.h"

AnimeStore::AnimeStore(QObject *parent) :
	CachingDataStore(parent)
{
	connect(this, &QtDataSync::CachingDataStoreBase::storeLoaded,
			this, [=](){
		emit animeInfoListChanged(loadAll());
	});
	connect(this, &QtDataSync::CachingDataStoreBase::dataChanged,
			this, [=](){
		emit animeInfoListChanged(loadAll());
	});
	connect(this, &QtDataSync::CachingDataStoreBase::dataResetted,
			this, [=](){
		emit animeInfoListChanged(loadAll());
	});
}
