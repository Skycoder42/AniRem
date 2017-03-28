#ifndef APIHELPER_H
#define APIHELPER_H

#include <QCoreApplication>
#include <genericrestreply.h>
#include <functional>

#include "core_global.h"
#include "proxerstatus.h"

class CORESHARED_EXPORT ApiHelper
{
	Q_DECLARE_TR_FUNCTIONS(ApiHelper)

public:
	static bool testValid(int code,
						  ProxerStatus status,
						  std::function<void(QString)> handler);
	template<typename DT, typename ET>
	static void setOnError(QtRestClient::GenericRestReply<DT, ET> *reply,
						   std::function<void(QString)> handler);

private:
	static QString transformError(ProxerStatus status, int);
	static void raiseError(const QString &error,
						   int errorCode,
						   QtRestClient::RestReply::ErrorType type,
						   std::function<void(QString)> handler);
};

template<typename DT, typename ET>
void ApiHelper::setOnError(QtRestClient::GenericRestReply<DT, ET> *reply, std::function<void (QString)> handler)
{
	reply->onAllErrors([=](QString s, int c, QtRestClient::RestReply::ErrorType t){
		raiseError(s, c, t, handler);
	}, &ApiHelper::transformError);
}

#endif // APIHELPER_H
