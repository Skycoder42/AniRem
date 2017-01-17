#include <coreapp.h>
#include <QApplication>
#include <QIcon>
#include "addanimedialog.h"
#include "mainwindow.h"
#include "widgetpresenter.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	WidgetPresenter::registerWidget<MainWindow>();
	WidgetPresenter::registerWidget<AddAnimeDialog>();

	return a.exec();
}
