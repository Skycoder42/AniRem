#include "proxerconnector.h"
#include <QtNetwork>

struct RequestInfo : public QObject {

	int id;
	QNetworkReply *pageReply;
	QNetworkReply *iconReply;

	QString title;
	QPixmap preview;

	inline RequestInfo(int id, QObject *parent) :
		QObject(parent),
		id(id),
		pageReply(nullptr),
		iconReply(nullptr),
		title(),
		preview()
	{}
};

ProxerConnector::ProxerConnector(QObject *parent) :
	QObject(parent),
	nam(new QNetworkAccessManager(this))
{}

void ProxerConnector::loadMetaData(int id)
{
	auto requestInfo = new RequestInfo(id, this);

	QNetworkRequest pageRequest(QUrl(QStringLiteral("http://proxer.me/info/%1/details").arg(id)));
	pageRequest.setOriginatingObject(requestInfo);
	pageRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	requestInfo->pageReply = this->nam->get(pageRequest);
	connect(requestInfo->pageReply, &QNetworkReply::finished,
			this, &ProxerConnector::metaRequestFinished,
			Qt::QueuedConnection);

	QNetworkRequest iconRequest(QUrl(QStringLiteral("http://cdn.proxer.me/cover/%1.jpg").arg(id)));
	iconRequest.setOriginatingObject(requestInfo);
	iconRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	requestInfo->iconReply = this->nam->get(iconRequest);
	connect(requestInfo->iconReply, &QNetworkReply::finished,
			this, &ProxerConnector::metaRequestFinished,
			Qt::QueuedConnection);
}

void ProxerConnector::metaRequestFinished()
{
	auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
	Q_ASSERT(reply);

	auto requestInfo = dynamic_cast<RequestInfo*>(reply->request().originatingObject());
	if(requestInfo) {
		try {
			if(reply->error() == QNetworkReply::NoError) {
				if(reply == requestInfo->pageReply){
					requestInfo->title = this->extractName(reply);
					requestInfo->pageReply = nullptr;
				} else if(reply == requestInfo->iconReply) {
					requestInfo->preview = this->extractImage(reply);
					requestInfo->iconReply = nullptr;
				}

				if(!requestInfo->pageReply && !requestInfo->iconReply){
					emit metaDataLoaded(requestInfo->id,
										requestInfo->title,
										requestInfo->preview);
					requestInfo->deleteLater();
				}
			} else if(reply->error() != QNetworkReply::OperationCanceledError)
				throw reply->errorString();
		} catch(QString error) {
			if(requestInfo->pageReply && requestInfo->pageReply->isRunning())
				requestInfo->pageReply->abort();
			if(requestInfo->iconReply && requestInfo->iconReply->isRunning())
				requestInfo->iconReply->abort();
			requestInfo->deleteLater();

			emit networkError(error);
		}
	}

	reply->deleteLater();
}

QString ProxerConnector::extractName(QNetworkReply *reply)
{
	auto rawData = QString::fromUtf8(reply->readAll());

	//check if "hentai"
	if(rawData.contains(QStringLiteral("<title>Info</title>")))
		throw tr("This tool can't add protected animes (e.g. Hentais)");
	else {
		//find the label
		auto start = rawData.indexOf(QStringLiteral("<td><b>Original Titel</b></td>"));
		auto beginTitle = rawData.indexOf(QStringLiteral("<td>"), start + 4) + 4;
		auto endTitle = rawData.indexOf(QStringLiteral("</td>"), beginTitle);
		if(start != -1 &&
		   beginTitle != -1 &&
		   endTitle != -1 &&
		   endTitle > beginTitle) {
			return rawData.mid(beginTitle, endTitle - beginTitle);
		} else
			throw tr("Unable to load anime name from webpage!");
	}
}

QPixmap ProxerConnector::extractImage(QNetworkReply *reply)
{
	QPixmap pm;
	if(pm.loadFromData(reply->readAll(), "jpg"))
		return pm;
	else
		throw tr("Unable to load preview image");
}
