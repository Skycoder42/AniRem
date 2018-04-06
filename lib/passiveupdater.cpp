#include "passiveupdater.h"
#include <QRemoteObjectReplica>

PassiveUpdater::PassiveUpdater(QObject *parent) :
	QObject(parent),
	_loader(nullptr),
	_notifier(nullptr),
	_manager(new QtDataSync::SyncManager(this)),
	_store(new AniremStore(this))
{}

void PassiveUpdater::performUpdateCheck()
{
	//WORKAROUND remove once patched
	qtmvvm_init();

	auto startFn = [this]() {
		_manager->runOnSynchronized([this](QtDataSync::SyncManager::SyncState state){
			Q_UNUSED(state)
			_loader->checkForUpdates(true);
		}, false);
	};

	if(_manager->replica()->isInitialized())
		startFn();
	else {
		connect(_manager->replica(), &QRemoteObjectReplica::initialized,
				this, startFn);
	}
}

void PassiveUpdater::qtmvvm_init()
{
	connect(_loader, &SeasonStatusLoader::completed,
			this, &PassiveUpdater::completed);
	connect(_loader, SIGNAL(updated(int,int)),
			dynamic_cast<QObject*>(_notifier), SLOT(updateProgress(int,int)));
}

void PassiveUpdater::completed(bool hasUpdates, const QString &errorString)
{
	if(!errorString.isEmpty()) {
		_notifier->showNotification(false, tr("Season check failed!"), errorString);
		emit done(true);
	} else {
		try {
			// show the notification
			auto allInfos = _store->loadAll();
			QStringList updatesList;
			for(const auto &anime : allInfos) {
				if(anime.hasNewSeasons()) {
					updatesList.append(tr("• %1 has now %n relation(s)", "", anime.totalSeasonCount())
									   .arg(anime.title()));
				}
			}

			if(!updatesList.isEmpty()) {
				_notifier->showNotification(true,
											tr("%n new relation(s) detected!", "", updatesList.size()),
											updatesList.join(QLatin1Char('\n')));
			}

			// if updated: sync then done, otherwise done
			if(hasUpdates) {
				_manager->runOnSynchronized([this](QtDataSync::SyncManager::SyncState state){
					Q_UNUSED(state)
					emit done(true);
				});
			} else
				emit done(!updatesList.isEmpty());
		} catch(QException &e) {
			qCritical() << "Failed to present update result with error" << e.what();
			_notifier->showNotification(false, tr("Season check failed!"), tr("Failed to load info of updated Animes"));
			emit done(true);
		}
	}
}
