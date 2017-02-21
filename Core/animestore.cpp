#include "animestore.h"
#include "coremessage.h"

AnimeStore::AnimeStore(QObject *parent) :
	QObject(parent),
	store(new QtDataSync::CachingDataStore<AnimeInfo*>(this))
{
	connect(store, &QtDataSync::CachingDataStoreBase::storeLoaded,
			this, [=](){
		emit animeInfoListChanged(store->loadAll());
		emit storeLoaded();
	});
	connect(store, &QtDataSync::CachingDataStoreBase::dataChanged,
			this, [=](){
		emit animeInfoListChanged(store->loadAll());
	});
}

AnimeList AnimeStore::animeInfoList() const
{
	return store->loadAll();
}

AnimeInfo *AnimeStore::animeInfo(int id) const
{
	return store->load(QString::number(id));
}

bool AnimeStore::containsAnime(int id) const
{
	return store->keys().contains(QString::number(id));
}

void AnimeStore::saveAnime(AnimeInfo *info)
{
	return store->save(info);
}

void AnimeStore::forgetAnime(int id)
{
	return store->remove(QString::number(id));
}

void AnimeStore::showError(const QString &error)
{
	CoreMessage::critical(tr("Data Error"), error);
}
