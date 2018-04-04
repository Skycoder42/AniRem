#ifndef SEASONSTATUSLOADER_H
#define SEASONSTATUSLOADER_H

#include <QObject>
#include <QQueue>
#include <QtMvvmCore/Injection>
#include "libanirem.h"
#include "infoclass.h"
#include "syncedsettings.h"

class SeasonStatusLoader : public QObject
{
	Q_OBJECT

	QTMVVM_INJECT_PROP(SyncedSettings*, settings, _settings)

public:
	Q_INVOKABLE explicit SeasonStatusLoader(QObject *parent = nullptr);

	bool isUpdating() const;

public slots:
	void checkForUpdates(bool forceHasUpdates = false);
	void checkForUpdates(const AnimeInfo &info);

signals:
	void started();
	void updated(int value, int max);
	void completed(bool hasUpdates, const QString &errorString = {});

private slots:
	void checkNext();
	void error(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType);

private:
	AniremStore *_store;
	InfoClass *_infoClass;
	SyncedSettings *_settings;

	QQueue<AnimeInfo> _updateQueue;
	int _lastMax;
	bool _anyUpdated;

	static AnimeInfo::SeasonType toType(const QString &medium);
	void addInfos(const QList<AnimeInfo> &infos);
};

#endif // SEASONSTATUSLOADER_H
