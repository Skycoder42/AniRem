#ifndef PROXERRELATIONS_H
#define PROXERRELATIONS_H

#include <QObject>
#include "proxerstatus.h"
#include "proxerentry.h"

class ProxerRelations : public ProxerStatus
{
	Q_OBJECT

	Q_PROPERTY(QList<ProxerEntryData*> data MEMBER data)
	QTRESTCLIENT_OBJECT_LIST_PROPERTY(data, ProxerEntryData*)

public:
	explicit ProxerRelations(QObject *parent = nullptr);

	QList<ProxerEntryData*> data;
};

Q_DECLARE_METATYPE(ProxerRelations*)

#endif // PROXERRELATIONS_H
