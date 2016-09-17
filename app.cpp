#include "app.h"
#include <dialogmaster.h>

App::App(int &argc, char **argv) :
	QApplication(argc, argv),
	store(nullptr),
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

	this->store = new AnimeStore(this);
	connect(this->store, &AnimeStore::loadingCompleted,
			this, &App::storeLoaded,
			Qt::QueuedConnection);
	connect(this->store, &AnimeStore::storeError, this, [this](QString error){
		this->showError(tr("Data Error"), error);
	}, Qt::QueuedConnection);

	this->trayIco = new QSystemTrayIcon(QApplication::windowIcon(), this);
	connect(this->trayIco, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason){
		if(reason == QSystemTrayIcon::Trigger)
			this->trayActivated();
	});
	connect(this->trayIco, &QSystemTrayIcon::messageClicked,
			this, &App::trayActivated);

	this->mainWindow = new MainWindow(this->store, nullptr);
}

App::~App()
{
	delete this->mainWindow;
}

int App::exec()
{
	QMetaObject::invokeMethod(this->store, "loadAnimes", Qt::QueuedConnection);
	this->mainWindow->open();
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
	this->mainWindow->loadingCompleted(infoList, true);

	//TODO if run in background mode
	this->trayIco->show();
}

int main(int argc, char *argv[])
{
	App a(argc, argv);
	return a.exec();
}
