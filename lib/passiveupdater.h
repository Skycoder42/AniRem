#ifndef PASSIVEUPDATER_H
#define PASSIVEUPDATER_H

#include <QObject>
#include <QtDataSync/SyncManager>
#include <QtMvvmCore/Injection>
#include "libanirem.h"
#include "seasonstatusloader.h"
#include "iupdatenotifier.h"

class PassiveUpdater : public QObject
{
	Q_OBJECT

	QTMVVM_INJECT_PROP(SeasonStatusLoader*, loader, _loader)
	QTMVVM_INJECT_PROP(IUpdateNotifier*, notifier, _notifier)

public:
	Q_INVOKABLE explicit PassiveUpdater(QObject *parent = nullptr);

public slots:
	void performUpdateCheck();

signals:
	void done(bool hasNotification);

private slots:
	void qtmvvm_init();

	void completed(bool hasUpdates, const QString &errorString);

private:
	SeasonStatusLoader *_loader;
	IUpdateNotifier *_notifier;
	QtDataSync::SyncManager *_manager;
	AniremStore *_store;
};

#endif // PASSIVEUPDATER_H
