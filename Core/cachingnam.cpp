#include "cachingnam.h"
#include <QNetworkRequest>
#include <QDir>
#include <QNetworkDiskCache>
#include <QStandardPaths>

CachingNam::CachingNam(QObject *parent) :
	QNetworkAccessManager(parent)
{
	auto cache = new QNetworkDiskCache(this);
	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setCacheDirectory(cacheDir.absoluteFilePath(QStringLiteral("./images")));
	setCache(cache);
}

QNetworkReply *CachingNam::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
	auto cRequest = request;
	cRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
	cRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	return QNetworkAccessManager::createRequest(op, cRequest, outgoingData);
}
