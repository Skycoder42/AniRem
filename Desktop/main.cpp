#include <QApplication>
#include <QIcon>
#include <qsingleinstance.h>
#include <settingsdialog.h>
#include <datasyncdialog.h>
#include <proxerapp.h>
#include <QTranslator>
#include <QLibraryInfo>
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

	//load translations
	auto translator = new QTranslator(&a);
	if(translator->load(QLocale(),
						QStringLiteral("seasonproxer_desktop"),
						QStringLiteral("_"),
						QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		a.installTranslator(translator);
	else {
		qDebug() << "Failed to load translator" << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
		delete translator;
	}

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
