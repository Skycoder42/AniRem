#ifndef PROXERRELATIONS_H
#define PROXERRELATIONS_H

#include "core_global.h"
#include <QObject>
#include "proxerstatus.h"
#include "proxerentry.h"

class CORESHARED_EXPORT ProxerRelations : public ProxerStatus
{
	Q_OBJECT

	Q_PROPERTY(QList<ProxerEntryData*> data MEMBER data)

public:
	Q_INVOKABLE ProxerRelations(QObject *parent = nullptr);

	QList<ProxerEntryData*> data;
};

Q_DECLARE_METATYPE(ProxerRelations*)

#endif // PROXERRELATIONS_H
