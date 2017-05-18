#include <QApplication>
#include <QIcon>
#include <qsingleinstance.h>
#include <settingsdialog.h>
#include <datasyncdialog.h>
#include <proxerapp.h>
#include "addanimedialog.h"
#include "detailsdockwidget.h"
#include "mainwindow.h"
#include "systemtraypresenter.h"

REGISTER_CORE_APP(ProxerApp)

int main(int argc, char *argv[])
{
	CoreApp::disableBoot();

	QApplication a(argc, argv);
	QApplication::setApplicationName(QStringLiteral(TARGET));
	QApplication::setApplicationVersion(QStringLiteral(VERSION));
	QApplication::setOrganizationName(QStringLiteral(COMPANY));
	QApplication::setOrganizationDomain(QStringLiteral("de.skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	QSingleInstance instance;
	instance.setStartupFunction([&](){
		WidgetPresenter::registerAsPresenter<SystemTrayPresenter>();
		WidgetPresenter::registerWidget<SettingsDialog>();
		DatasyncDialog::registerWidgets();
		WidgetPresenter::registerWidget<MainWindow>();
		WidgetPresenter::registerWidget<AddAnimeDialog>();
		WidgetPresenter::registerWidget<DetailsDockWidget>();
		CoreApp::instance()->bootApp();

		QObject::connect(&instance, &QSingleInstance::instanceMessage,
						 coreApp, &ProxerApp::showMainControl);

		return EXIT_SUCCESS;
	});

	return instance.singleExec();
}
