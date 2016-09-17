#include "app.h"
#include <QCommandLineParser>
#include <dialogmaster.h>

App::App(int &argc, char **argv) :
	QApplication(argc, argv),
	store(nullptr),
	isUpdateMode(false),
	trayIco(nullptr),
	mainWindow(nullptr)
{
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	qRegisterMetaType<AnimeInfo>();
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
	QMetaObject::invokeMethod(this->store, "loadAnimes", Qt::QueuedConnection);
	return QApplication::exec();
}

void App::showError(QString title, QString message)
{
	DialogMaster::critical(this->activeWindow(), message, title);
}

void App::trayActivated()
{
	this->trayIco->hide();
	this->mainWindow->open();
}

void App::storeLoaded(QList<AnimeInfo> infoList)
{
	if(this->isUpdateMode) {
		this->mainWindow->loadingCompleted(infoList, false);

		//TODO if run in background mode
		this->initTray();
	} else
		this->mainWindow->loadingCompleted(infoList, true);
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

	this->mainWindow = new MainWindow(this->store, nullptr);
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

	this->trayIco->show();
}

int main(int argc, char *argv[])
{
	App a(argc, argv);
	return a.exec();
}
