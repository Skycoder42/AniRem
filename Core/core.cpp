#include "core.h"
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QStandardPaths>

const QString Core::ProxerRest(QStringLiteral("proxer"));

class CachingNam : public QNetworkAccessManager
{
public:
	CachingNam(QObject *parent);
protected:
	QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) override;
};

QNetworkAccessManager *Core::createImageLoaderNam(QObject *parent)
{
	auto nam = new CachingNam(parent);
	auto cache = new QNetworkDiskCache(nam);
	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	cache->setCacheDirectory(cacheDir.absoluteFilePath(QStringLiteral("./netcache")));
	nam->setCache(cache);
	return nam;
}

CachingNam::CachingNam(QObject *parent) :
	QNetworkAccessManager(parent)
{}

QNetworkReply *CachingNam::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
	auto cRequest = request;
	cRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
	cRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	return QNetworkAccessManager::createRequest(op, cRequest, outgoingData);
}
