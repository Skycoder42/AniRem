#include "infoclass.h"
#include <QtRestClient>
#include "core.h"
#include "proxerentry.h"
using namespace QtRestClient;

InfoClass::InfoClass(QObject *parent) :
	QObject(parent),
	restClass(RestClient::createApiClass(Core::ProxerRest, QStringLiteral("info"), this))
{}

QtRestClient::GenericRestReply<ProxerEntry, ProxerStatus> *InfoClass::getEntry(int id)
{
	return restClass->get<ProxerEntry, ProxerStatus>(QStringLiteral("entry"), CONCAT_PARAMS("id", id))
			->enableAutoDelete()
			->onAllErrors([=](RestReply *, QString error, int, RestReply::ErrorType type){
				raiseError(type, error);
	}, &InfoClass::transformError);
}

QtRestClient::GenericRestReply<ProxerRelations, ProxerStatus> *InfoClass::getRelations(int id)
{
	return restClass->get<ProxerRelations, ProxerStatus>(QStringLiteral("relations"), CONCAT_PARAMS("id", id))
			->enableAutoDelete()
			->onAllErrors([=](RestReply *, QString error, int, RestReply::ErrorType type){
				raiseError(type, error);
	}, &InfoClass::transformError);
}

bool InfoClass::testValid(int code, ProxerStatus *status)
{
	if(status->error == 0 && (code / 100) == 2)
		return true;
	else {
		raiseError(RestReply::FailureError, status->message);
		return false;
	}
}

QString InfoClass::transformError(ProxerStatus *status, int)
{
	return status->message;
}

void InfoClass::raiseError(RestReply::ErrorType type, const QString &error)
{
	switch (type) {
	case RestReply::NetworkError:
		emit apiError(tr("Network Error: ") + error);
		break;
	case RestReply::JsonParseError:
		emit apiError(tr("Data Error: ") + error);
		break;
	case RestReply::FailureError:
		emit apiError(tr("Proxer-API Error: ") + error);
		break;
	case RestReply::DeserializationError:
		emit apiError(tr("Data Error: ") + error);
		break;
	default:
		Q_UNREACHABLE();
	}
}
