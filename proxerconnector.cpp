#include "proxerconnector.h"
#include <QtNetwork>
#include <QRegularExpression>

#define PAGE_ERROR tr("Invalid webpage format. Can't load data from proxer.me!")

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

void ProxerConnector::loadSeasonCount(int id)
{
	QNetworkRequest request(QUrl(QStringLiteral("http://proxer.me/info/%1/relation").arg(id)));
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	auto reply = this->nam->get(request);
	reply->setProperty("id", id);
	connect(reply, &QNetworkReply::finished,
			this, &ProxerConnector::seasonRequestFinished,
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

void ProxerConnector::seasonRequestFinished()
{
	auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
	Q_ASSERT(reply);

	try {
		if(reply->error() == QNetworkReply::NoError) {
			auto rawData = QString::fromUtf8(reply->readAll());
			this->checkHentai(rawData);

			auto seasonCount = 0;
			auto tBegin = this->findIndex(rawData, QStringLiteral("<table id=\"box-table-a\""));
			while(tBegin != -1) {
				auto tEnd = this->findIndex(rawData, "</table>", tBegin, true);

				auto rBegin = this->findIndex(rawData, "<tr>", tBegin, true);
				forever {
					rBegin = this->findIndex(rawData, "<tr", rBegin + 4, true);
					if(rBegin == -1 || rBegin > tEnd)
						break;

					//go to 4th td
					auto begin = this->findIndex(rawData, "<td>", rBegin);//1
					begin = this->findIndex(rawData, "<td>", begin + 4);//2
					begin = this->findIndex(rawData, "<td>", begin + 4);//3
					begin = this->findIndex(rawData, "<td>", begin + 4);//4
					auto end = this->findIndex(rawData, "</td>", begin);

					begin += 4;
					auto type = rawData.mid(begin, end - begin);
					//TODO check type accepted --> settings
					qDebug() << "Found seasons for" << reply->property("id").toInt() << "of type" << type;
					seasonCount++;
				}

				tBegin = rawData.indexOf(QStringLiteral("<table id=\"box-table-a\""), tBegin + 1);
			}

			emit seasonsLoaded(reply->property("id").toInt(), seasonCount);
		} else
			throw reply->errorString();
	} catch(QString error) {
		emit networkError(error);
	}

	reply->deleteLater();
}

QString ProxerConnector::extractName(QNetworkReply *reply)
{
	auto rawData = QString::fromUtf8(reply->readAll());
	this->checkHentai(rawData);

	//find the label
	auto start = this->findIndex(rawData, QStringLiteral("<td><b>Original Titel</b></td>"));
	auto beginTitle = this->findIndex(rawData, QStringLiteral("<td>"), start + 4) + 4;
	auto endTitle = this->findIndex(rawData, QStringLiteral("</td>"), beginTitle);
	if(endTitle > beginTitle)
		return rawData.mid(beginTitle, endTitle - beginTitle);
	else
		throw PAGE_ERROR;
}

QPixmap ProxerConnector::extractImage(QNetworkReply *reply)
{
	QPixmap pm;
	if(pm.loadFromData(reply->readAll(), "jpg"))
		return pm;
	else
		throw tr("Unable to load preview image");
}

int ProxerConnector::findIndex(QString raw, QString target, int offset, bool allowsError) const
{
	auto index = raw.indexOf(target, offset);
	if(index == -1 && !allowsError)
		throw PAGE_ERROR;
	else
		return index;
}

void ProxerConnector::checkHentai(QString raw) const
{
	if(raw.contains(QStringLiteral("<title>Info</title>")))
		throw tr("This tool can't add protected animes (e.g. Hentais)");
}
