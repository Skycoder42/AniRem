#include "proxerconnector.h"
#include "proxer-api-key.h"
#include "ProxerApi/proxerentry.h"
#include <QtRestClient>
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

	infoClass = client->createClass("info", this);
}

void ProxerConnector::loadMetaData(int id)
{
	auto request = new MetaRequest(this);

	infoClass->get<ProxerEntry, ProxerStatus>("entry", RestClass::concatParameters("id", id))
			->enableAutoDelete()
			->onSucceeded([=](RestReply*, int, ProxerEntry *entry){
				if(entry->error == 0)
					request->entry = entry;
				else
					emit apiError(tr("Proxer-API Error: ") + entry->message);
				request->completor++;
				tryFinishMetaRequest(request);
			})->onFailed([=](RestReply*, int, ProxerStatus *status){
				emit apiError(tr("Proxer-API Error: ") + status->message);
				request->completor++;
				tryFinishMetaRequest(request);
			})->onError([=](RestReply*, QString error, int, RestReply::ErrorType){
				emit apiError(tr("Network Error: ") + error);
				request->completor++;
				tryFinishMetaRequest(request);
			})->onSerializeException([=](RestReply*, SerializerException exception){
				emit apiError(tr("Data Error: ") + exception.qWhat());
				request->completor++;
				tryFinishMetaRequest(request);
			});


	QNetworkRequest iconRequest(QUrl(QStringLiteral("http://cdn.proxer.me/cover/%1.jpg").arg(id)));
	iconRequest.setOriginatingObject(request);
	iconRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	auto reply = this->client->manager()->get(iconRequest);
	connect(reply, &QNetworkReply::finished,
			this, &ProxerConnector::imageReplyFinished);
}

void ProxerConnector::loadSeasonCount(int id)
{

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
