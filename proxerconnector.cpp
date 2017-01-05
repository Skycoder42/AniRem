#include "proxerconnector.h"
#include "proxer-api-key.h"
using namespace QtRestClient;

ProxerConnector::ProxerConnector(QObject *parent) :
	QObject(parent),
	client(new RestClient(this))
{
	client->setBaseUrl(QStringLiteral("https://proxer.me/api"));
	client->setApiVersion(1);
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);
}

void ProxerConnector::loadMetaData(int id)
{

}

void ProxerConnector::loadSeasonCount(int id)
{

}
