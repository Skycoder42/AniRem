#ifndef INFOCLASS_H
#define INFOCLASS_H

#include "core_global.h"
#include "proxerstatus.h"
#include "proxerentry.h"
#include "proxerrelations.h"

#include <QObject>
#include <animeinfo.h>
#include <functional>
#include <restclass.h>

class CORESHARED_EXPORT InfoClass : public QObject
{
	Q_OBJECT
public:
	explicit InfoClass(QObject *parent = nullptr);

	QtRestClient::GenericRestReply<ProxerEntry, ProxerStatus> *getEntry(int id);
	QtRestClient::GenericRestReply<ProxerRelations, ProxerStatus> *getRelations(int id);

	bool testValid(int code, ProxerStatus status);

signals:
	void apiError(QString error);

private:
	QtRestClient::RestClass *restClass;

	static QString transformError(ProxerStatus status, int);
	void raiseError(QtRestClient::RestReply::ErrorType type, int errorCode, const QString &error);
};

#endif // INFOCLASS_H
