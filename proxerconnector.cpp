#include "proxerconnector.h"
#include "proxer-api-key.h"
#include "ProxerApi/proxerentry.h"
#include <QtRestClient>
#include <ProxerApi/proxerrelations.h>
using namespace QtRestClient;

struct MetaRequest : public QObject
{
	ProxerEntry *entry;
	QPixmap pixmap;
	int completor;

	MetaRequest(QObject *parent) :
		QObject(parent),
		entry(nullptr),
		pixmap(),
		completor(0)
	{}
};

ProxerConnector::ProxerConnector(QObject *parent) :
	QObject(parent),
	client(new RestClient(this)),
	infoClass(nullptr)
{
	client->setBaseUrl(QStringLiteral("https://proxer.me/api"));
	client->setApiVersion({1});
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);
	client->serializer()->setAllowDefaultNull(true);//TODO use this to provoke an error to test error handling

	infoClass = client->createClass("info", this);
}

void ProxerConnector::loadMetaData(int id)
{
	auto request = new MetaRequest(this);

	infoClass->get<ProxerEntry, ProxerStatus>("entry", CONCAT_PARAMS("id", id))
			->enableAutoDelete()
			->onSucceeded([=](RestReply*, int, ProxerEntry *entry){
				if(entry->error == 0)
					request->entry = entry;
				else
					emit apiError(tr("Proxer-API Error: ") + entry->message);
				request->completor++;
				tryFinishMetaRequest(request);
	})->onAllErrors([=](RestReply *, QString error, int, RestReply::ErrorType type){
		formatError(error, type);
		request->completor++;
		tryFinishMetaRequest(request);
	}, &ProxerConnector::tranformError);


	QNetworkRequest iconRequest(QUrl(QStringLiteral("http://cdn.proxer.me/cover/%1.jpg").arg(id)));
	iconRequest.setOriginatingObject(request);
	iconRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	auto reply = this->client->manager()->get(iconRequest);
	connect(reply, &QNetworkReply::finished,
			this, &ProxerConnector::imageReplyFinished);
}

void ProxerConnector::loadSeasonCount(int id)
{
	infoClass->get<ProxerRelations, ProxerStatus>("relations", CONCAT_PARAMS("id", id))
			->enableAutoDelete()
			->onSucceeded([=](RestReply*, int, ProxerRelations *entry){
				if(entry->error == 0)
					emit seasonsLoaded(id, entry->data.count());
				else
					emit apiError(tr("Proxer-API Error: ") + entry->message);
			})->onAllErrors([this](RestReply *, QString error, int, RestReply::ErrorType type){
				formatError(error, type);
			}, &ProxerConnector::tranformError);
}

void ProxerConnector::imageReplyFinished()
{
	auto reply = qobject_cast<QNetworkReply*>(sender());
	Q_ASSERT(reply);
	Q_ASSERT(dynamic_cast<MetaRequest*>(reply->request().originatingObject()));
	auto request = static_cast<MetaRequest*>(reply->request().originatingObject());

	if(reply->error() == QNetworkReply::NoError) {
		QPixmap pm;
		if(pm.loadFromData(reply->readAll(), "jpg"))
			request->pixmap = pm;
		else
			emit apiError(tr("Unable to load preview image"));
	} else
		emit apiError(tr("Network Error: ") + reply->errorString());

	request->completor++;
	tryFinishMetaRequest(request);
}

void ProxerConnector::tryFinishMetaRequest(MetaRequest *request)
{
	if(request->completor < 2)
		return;
	if(request->entry && !request->pixmap.isNull()) {
		emit metaDataLoaded(request->entry->data->id,
							request->entry->data->name,
							request->pixmap);
	}
}

QString ProxerConnector::tranformError(ProxerStatus *status, int)
{
	return status->message;
}

void ProxerConnector::formatError(const QString &error, RestReply::ErrorType type)
{
	switch (type) {
	case QtRestClient::RestReply::NetworkError:
		emit apiError(tr("Network Error: ") + error);
		break;
	case QtRestClient::RestReply::JsonParseError:
		emit apiError(tr("Data Error: ") + error);
		break;
	case QtRestClient::RestReply::FailureError:
		emit apiError(tr("Proxer-API Error: ") + error);
		break;
	case QtRestClient::RestReply::DeserializationError:
		emit apiError(tr("Data Error: ") + error);
		break;
	default:
		Q_UNREACHABLE();
	}
}
