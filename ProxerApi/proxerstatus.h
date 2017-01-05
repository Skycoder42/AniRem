#ifndef PROXERSTATUS_H
#define PROXERSTATUS_H

#include <QObject>
#include <restobject.h>

class ProxerStatus : public QtRestClient::RestObject
{
	Q_OBJECT

	Q_PROPERTY(int error MEMBER error)
	Q_PROPERTY(QString message MEMBER message)
	Q_PROPERTY(int code MEMBER code)

public:
	Q_INVOKABLE ProxerStatus(QObject *parent = nullptr);

	int error;
	QString message;
	int code;
};

Q_DECLARE_METATYPE(ProxerStatus*)

#endif // PROXERSTATUS_H
