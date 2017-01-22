#include "proxerapp.h"
#include <QtRestClient>
#include "core.h"
#include "coremessage.h"
#include "animeinfo.h"
#include "proxer-api-key.h"

REGISTER_CORE_APP(ProxerApp)

ProxerApp::ProxerApp(QObject *parent) :
	CoreApp(parent),
	store(nullptr),
	loader(nullptr),
	mainControl(nullptr)
{
	qRegisterMetaType<AnimeList>();
	QtRestClient::registerListConverters<ProxerEntryData*>();
}

void ProxerApp::checkForSeasonUpdates()
{
	loader->checkForUpdates(store->animeInfoList());
}

void ProxerApp::setupParser(QCommandLineParser &parser, bool &allowInvalid)
{
	Q_UNIMPLEMENTED();
	CoreApp::setupParser(parser, allowInvalid);
}

bool ProxerApp::startApp(const QCommandLineParser &parser)
{
	if(autoShowHelpOrVersion(parser))
		return true;

	//create proxer rest api
	auto client = new QtRestClient::RestClient(qApp);
	client->setBaseUrl(QStringLiteral("https://proxer.me/api"));
	client->setApiVersion({1});
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);
	client->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling
	QtRestClient::RestClient::addGlobalApi(Core::ProxerRest, client);

	//anime data store
	store = new AnimeStore(this);
	connect(store, &AnimeStore::loadingChanged,
			this, &ProxerApp::storeLoaded,
			Qt::QueuedConnection);

	loader = new SeasonStatusLoader(this);
	connect(loader, &SeasonStatusLoader::newSeasonsDetected,
			store, &AnimeStore::saveAnime);

	mainControl = new MainControl(store, this);
	connect(loader, &SeasonStatusLoader::updated,
			mainControl, &MainControl::setProgress);
	connect(loader, &SeasonStatusLoader::completed,
			this, &ProxerApp::updateDone);

	showControl(mainControl);
	return true;
}

void ProxerApp::aboutToQuit()
{

}

void ProxerApp::storeLoaded(bool loading)
{
	mainControl->updateLoadStatus(loading);
}

void ProxerApp::updateDone(QString errorString)
{
	mainControl->updateLoadStatus(false);
	if(!errorString.isNull())
		CoreMessage::critical(tr("Season check failed"), errorString);
}
