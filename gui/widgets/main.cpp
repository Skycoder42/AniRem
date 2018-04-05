#include <QtWidgets/QApplication>
#include <QtMvvmWidgets/WidgetsPresenter>
#include <QtMvvmDataSyncWidgets/qtmvvmdatasyncwidgets_global.h>
#include <aniremapp.h>

#include "mainwindow.h"
#include "addanimedialog.h"
#include "detailsdockwidget.h"
#include "widgetsupdatenotifier.h"

// Register the core app
QTMVVM_REGISTER_CORE_APP(AniRemApp)

int main(int argc, char *argv[])
{
	//TODO test without icon theme
	QApplication a(argc, argv);

	QtMvvm::ServiceRegistry::instance()->registerInterface<IUpdateNotifier, WidgetsUpdateNotifier>();

	QtMvvm::registerDataSyncWidgets();
	QtMvvm::WidgetsPresenter::registerView<MainWindow>();
	QtMvvm::WidgetsPresenter::registerView<AddAnimeDialog>();
	QtMvvm::WidgetsPresenter::registerView<DetailsDockWidget>();

	return a.exec();
}
