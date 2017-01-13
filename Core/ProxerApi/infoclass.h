#ifndef INFOCLASS_H
#define INFOCLASS_H

#include "core_global.h"
#include "proxerentry.h"
#include "proxerstatus.h"

#include <QObject>
#include <animeinfo.h>
#include <functional>
#include <restclass.h>

class CORESHARED_EXPORT InfoClass : public QObject
{
	Q_OBJECT
public:
	explicit InfoClass(QObject *parent = nullptr);

	void getEntry(int id, std::function<void(AnimePtr)> replyHandler);
	QtRestClient::GenericRestReply<ProxerEntry, ProxerStatus> *getEntry(int id);

	bool testValid(int code, ProxerStatus *status);

signals:
	void apiError(QString error);

private:
	QtRestClient::RestClass *restClass;

	static QString transformError(ProxerStatus *status, int);
	void raiseError(QtRestClient::RestReply::ErrorType type, const QString &error);
};

#endif // INFOCLASS_H
