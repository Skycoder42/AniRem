#include "app.h"
#include <QCommandLineParser>
#include <QTimer>
#include <QtRestClient>
#include <dialogmaster.h>
#include <ProxerApi/proxerentry.h>

App::App(int &argc, char **argv) :
	QApplication(argc, argv),
	store(nullptr),
	loader(nullptr),
	isUpdateMode(false),
	mainWindow(nullptr),
	trayIco(nullptr),
	trayHigh(false)
{
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	qRegisterMetaType<AnimeInfo>();
	qRegisterMetaType<QList<AnimeInfo>>();
	QtRestClient::registerListConverters<ProxerEntryData*>();
}

App::~App()
{
	delete this->mainWindow;
}

int App::exec()
{
	QCommandLineParser parser;
	parser.addVersionOption();
	parser.addHelpOption();
	parser.setApplicationDescription(tr("A tool to passivly check for updates on seasons, for proxer.me"));
	parser.addOption({
						 {"u", "update"},
						 tr("Runs the app in updater mode: It will check for updated seasons and either show a "
							"notification, if there are any, or just silently quit")
					 });

	parser.process(*this);
	this->isUpdateMode = parser.isSet("update");

	this->init();

	if(!this->isUpdateMode)
		this->mainWindow->open();
	this->mainWindow->showStatus(tr("Loading saved animes…"), true);
	return QApplication::exec();
}

void App::showError(QString title, QString message)
{
	DialogMaster::critical(this->activeWindow(), message, title);
}

void App::trayActivated()
{
	this->trayIco->hide();
	this->trayIco->deleteLater();
	this->trayIco = nullptr;
	this->mainWindow->open();
	this->isUpdateMode = false;
}

void App::reload()
{
	this->mainWindow->updateLoadStatus(false);
	this->mainWindow->showStatus(tr("Checking for new seasons…"), true);
	this->loader->load();
}

void App::storeLoaded()
{
	if(this->isUpdateMode)
		this->reload();
	else
		this->mainWindow->updateLoadStatus(true);
}

void App::seasonsLoaded(bool hasNew)
{
	this->mainWindow->updateLoadStatus(true);

	if(this->isUpdateMode) {
		if(hasNew)
			this->initTray();
		else
			qApp->quit();
	} else {
		this->alert(this->mainWindow);
		this->beep();
	}
}

void App::init()
{
	this->store = new AnimeStore(this);
	connect(this->store, &AnimeStore::loadingCompleted,
			this, &App::storeLoaded,
			Qt::QueuedConnection);
	connect(this->store, &AnimeStore::storeError, this, [this](QString error){
		this->showError(tr("Data Error"), error);
	}, Qt::QueuedConnection);

	this->loader = new SeasonStatusLoader(this->store, this);
	connect(this->loader, &SeasonStatusLoader::completed,
			this, &App::seasonsLoaded);

	this->mainWindow = new MainWindow(this->store, nullptr);
	connect(this->mainWindow, &MainWindow::reload,
			this, &App::reload);
	connect(this->loader, &SeasonStatusLoader::updated,
			this->mainWindow, &MainWindow::setProgress);
}

void App::initTray()
{
	this->trayIco = new QSystemTrayIcon(QApplication::windowIcon(), this);
	connect(this->trayIco, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason){
		if(reason == QSystemTrayIcon::Trigger)
			this->trayActivated();
	});
	connect(this->trayIco, &QSystemTrayIcon::messageClicked,
			this, &App::trayActivated);

	auto trayIcoTimer = new QTimer(this->trayIco);
	connect(trayIcoTimer, &QTimer::timeout, this->trayIco, [this](){
		if(this->trayHigh)
			this->trayIco->setIcon(QIcon(QStringLiteral(":/icons/main.ico")));
		else
			this->trayIco->setIcon(QIcon(QStringLiteral(":/icons/inverted.ico")));
		this->trayHigh = !this->trayHigh;
	});
	trayIcoTimer->start(500);

	this->trayIco->show();
	this->trayIco->showMessage(tr("New Seasons!"),
							   tr("One or more animes have new seasons! Click this message "
								  "to show the animes."),
							   QSystemTrayIcon::Information);
}

int main(int argc, char *argv[])
{
	App a(argc, argv);
	return a.exec();
}
