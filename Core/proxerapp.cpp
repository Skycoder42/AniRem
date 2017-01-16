#include "proxerapp.h"
#include <QtRestClient>
#include "core.h"
#include "animeinfo.h"
#include "proxer-api-key.h"

REGISTER_CORE_APP(ProxerApp)

ProxerApp::ProxerApp(QObject *parent) :
	CoreApp(parent),
	store(nullptr),
	mainControl(nullptr)
{
	qRegisterMetaType<AnimeList>();
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
	client->addGlobalHeader("proxer-api-key", PROXER_API_KEY);//TODO "hide" key
	client->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling
	QtRestClient::RestClient::addGlobalApi(Core::ProxerRest, client);

	//anime data store
	store = new AnimeStore(this);
	connect(store, &AnimeStore::loadingCompleted,
			this, &ProxerApp::storeLoaded,
			Qt::QueuedConnection);

	mainControl = new MainControl(store, this);

	return showControl(mainControl);
}

void ProxerApp::aboutToQuit()
{

}

void ProxerApp::storeLoaded()
{
	emit mainControl->updateLoadStatus(true);
}
