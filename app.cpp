#include "app.h"

App::App(int &argc, char **argv) :
	QApplication(argc, argv),
	mainWindow(nullptr)
{
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	qRegisterMetaType<AnimeInfo>();

	this->mainWindow = new MainWindow(nullptr);
}

App::~App()
{
	delete this->mainWindow;
}

int App::exec()
{
	this->mainWindow->show();
	return QApplication::exec();
}

int main(int argc, char *argv[])
{
	App a(argc, argv);
	return a.exec();
}
