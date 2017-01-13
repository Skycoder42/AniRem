#include "app.h"
#include <coreapp.h>

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
	showMessage(QMessageBox::Critical, title, message);
}

void App::showMessage(QMessageBox::Icon icon, QString title, QString message)
{
	switch(icon) {
	case QMessageBox::NoIcon:
	case QMessageBox::Information:
		DialogMaster::information(activeWindow(), message, title);
		break;
	case QMessageBox::Warning:
		DialogMaster::warning(activeWindow(), message, title);
		break;
	case QMessageBox::Critical:
		DialogMaster::critical(activeWindow(), message, title);
		break;
	case QMessageBox::Question:
		DialogMaster::question(activeWindow(), message, title);
		break;
	default:
		Q_UNREACHABLE();
		break;
	}
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
	CoreApp::setMainPresenter(nullptr);//TODO
	return a.exec();
}
