#include "app.h"

#include <QIcon>
#include <dialogmaster.h>

App::App(int &argc, char **argv) :
	QApplication(argc, argv),
	store(nullptr),
	imgLoader(nullptr),
	mainWindow(nullptr)
{
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));
}

App::~App()
{
	delete mainWindow;
}

ImageLoader *App::imageLoader() const
{
	return imgLoader;
}

int App::exec()
{
	init();

	mainWindow->open();
	mainWindow->showStatus(tr("Loading saved animes…"), true);
	return QApplication::exec();
}

void App::showError(QString title, QString message)
{
	DialogMaster::critical(activeWindow(), message, title);
}

void App::reload()
{
	mainWindow->updateLoadStatus(false);
	mainWindow->showStatus(tr("Checking for new seasons…"), true);
}

void App::storeLoaded()
{
	mainWindow->updateLoadStatus(true);
}

void App::init()
{
	store = new AnimeStore(this);
	connect(store, &AnimeStore::loadingCompleted,
			this, &App::storeLoaded,
			Qt::QueuedConnection);
	connect(store, &AnimeStore::storeError, this, [this](QString error){
		showError(tr("Data Error"), error);
	}, Qt::QueuedConnection);

	imgLoader = new ImageLoader(this);

	mainWindow = new MainWindow(store, nullptr);
	connect(mainWindow, &MainWindow::reload,
			this, &App::reload);
}

int main(int argc, char *argv[])
{
	App a(argc, argv);
	return a.exec();
}
