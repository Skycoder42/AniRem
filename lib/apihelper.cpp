#include "apihelper.h"
#include <QDebug>
#include <localsettings.h>
#include "libanirem.h"
using namespace QtRestClient;

QString ApiHelper::transformError(ProxerStatus status, int)
{
	return status.message();
}

QString ApiHelper::formatError(const QString &error, int errorCode, RestReply::ErrorType type)
{
	qWarning() << "API-Error type"
			   << type
			   << "code"
			   << errorCode
			   << "with error string"
			   << error;

	switch (type) {
	case RestReply::NetworkError:
		return tr("Network Error: <i>%1</i>").arg(error);
	case RestReply::JsonParseError:
		return tr("Data Error: <i>%1</i>").arg(error);
	case RestReply::FailureError:
		if(errorCode == 1005) {
			AniRem::setProxerToken({}, LocalSettings::instance());
			return tr("Invalid Login-Token! The old token has been removed. Please try again.");
		} else
			return tr("Proxer-API Error: <i>%1</i>").arg(error);
	case RestReply::DeserializationError:
		return tr("Data Error: <i>%1</i>").arg(error);
	default:
		Q_UNREACHABLE();
		return QString();
	}
}

bool ApiHelper::testValid(int code, ProxerStatus status)
{
	return status.error() == 0 && (code / 100) == 2;
}
