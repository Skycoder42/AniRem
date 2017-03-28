#include "apihelper.h"
#include <QDebug>
using namespace QtRestClient;

bool ApiHelper::testValid(int code, ProxerStatus status, std::function<void(QString)> handler)
{
	if(status.error() == 0 && (code / 100) == 2)
		return true;
	else {
		raiseError(status.message(), code, RestReply::FailureError, handler);
		return false;
	}
}

QString ApiHelper::transformError(ProxerStatus status, int)
{
	return status.message();
}

void ApiHelper::raiseError(const QString &error, int errorCode, QtRestClient::RestReply::ErrorType type, std::function<void(QString)> handler)
{
	qWarning() << "API-Error type"
			   << type
			   << "code"
			   << errorCode
			   << "with error string"
			   << error;

	switch (type) {
	case RestReply::NetworkError:
		emit handler(tr("Network Error: ") + error);
		break;
	case RestReply::JsonParseError:
		emit handler(tr("Data Error: ") + error);
		break;
	case RestReply::FailureError:
		emit handler(tr("Proxer-API Error: ") + error);
		break;
	case RestReply::DeserializationError:
		emit handler(tr("Data Error: ") + error);
		break;
	default:
		Q_UNREACHABLE();
	}
}
