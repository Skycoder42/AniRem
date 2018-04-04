#ifndef APIHELPER_H
#define APIHELPER_H

#include <QCoreApplication>
#include <genericrestreply.h>
#include <functional>

#include "proxerstatus.h"

class ApiHelper
{
	Q_DECLARE_TR_FUNCTIONS(ApiHelper)

public:
	static QString transformError(ProxerStatus status, int);
	static QString formatError(const QString &error, int errorCode, QtRestClient::RestReply::ErrorType type);
	static bool testValid(int code, ProxerStatus status);
};

#endif // APIHELPER_H
