#include "core.h"
#include <QCoreApplication>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include <QtRestClient>
#include "proxer-api-key.h"

#include "animeinfo.h"

class CachingNam : public QNetworkAccessManager
{
public:
	CachingNam(QObject *parent);
protected:
	QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) override;
};

static void startup();
Q_COREAPP_STARTUP_FUNCTION(startup)

const QString Core::ProxerRest(QStringLiteral("proxer"));

static void startup()
{
	qRegisterMetaType<AnimePtr>();
	qRegisterMetaType<AnimeList>();
}

void Core::createProxerApi()
{
	auto client = new QtRestClient::RestClient(qApp);
	client->setBaseUrl(QStringLiteral("https://proxer.me/api"));
	client->setApiVersion({1});
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);//TODO "hide" key
	client->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling
	QtRestClient::RestClient::addGlobalApi(Core::ProxerRest, client);
}

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
