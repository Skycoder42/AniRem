#include <QApplication>
#include <QIcon>
#include <QStandardPaths>
#include <QDir>
#include <dialogmaster.h>
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
		QSettings settings;
		if(settings.value(QStringLiteral("migrate")).toBool()) {
			settings.setValue(QStringLiteral("migrate"), false);
			QDir ardir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
			auto goalPath = ardir.absoluteFilePath(QStringLiteral("./qtdatasync_localstore"));
			auto oldPath = ardir.absoluteFilePath(QStringLiteral("./qtdatasync_localstore.old"));
			try {
				if(!ardir.rename(goalPath, oldPath))
					throw QCoreApplication::translate("GLOBAL", "Failed to backup the original files.");
				QDir spdir = QDir(ardir.absolutePath().replace(QStringLiteral("AniRem"), QStringLiteral("SeasonProxer")));
				if(!spdir.exists())
					throw QCoreApplication::translate("GLOBAL", "Failed to find the data of SeasonProxer. Did you use seasonproxer?");
				if(!spdir.rename(QStringLiteral("./qtdatasync_localstore"), goalPath))
					throw QCoreApplication::translate("GLOBAL", "Failed to move the data from SeasonProxer to AnimRem.");
				DialogMaster::information(nullptr, QCoreApplication::translate("GLOBAL", "Migration successfully completed!"));
			} catch(QString error) {
				ardir.rename(oldPath, goalPath);
				DialogMaster::critical(nullptr, error, QCoreApplication::translate("GLOBAL", "Migration failed!"));
			}
		}

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
