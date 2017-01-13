#ifndef PROXERENTRY_H
#define PROXERENTRY_H

#include "core_global.h"
#include <QObject>
#include "proxerstatus.h"

class CORESHARED_EXPORT ProxerEntryData : public QtRestClient::RestObject
{
	Q_OBJECT

	Q_PROPERTY(int id MEMBER id)
	Q_PROPERTY(QString name MEMBER name)
	Q_PROPERTY(QString medium MEMBER medium)
	//all other are currently not required

	//special properties from seasons
	Q_PROPERTY(int year MEMBER year)
	Q_PROPERTY(int season MEMBER season)

public:
	Q_INVOKABLE ProxerEntryData(QObject *parent = nullptr);

	int id;
	QString name;
	QString medium;
	int year;
	int season;
};

class CORESHARED_EXPORT ProxerEntry : public ProxerStatus
{
	Q_OBJECT

	Q_PROPERTY(ProxerEntryData* data MEMBER data)

public:
	Q_INVOKABLE ProxerEntry(QObject *parent = nullptr);

	ProxerEntryData* data;
};

Q_DECLARE_METATYPE(ProxerEntryData*)
Q_DECLARE_METATYPE(ProxerEntry*)

#endif // PROXERENTRY_H
