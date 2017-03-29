#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QSettings>
#include "addanimedialog.h"

MainWindow::MainWindow(Control *mControl, QWidget *parent) :
	QMainWindow(parent),
	control(static_cast<MainControl*>(mControl)),
	ui(new Ui::MainWindow),
	statusLabel(new QLabel(tr("Checking for new seasons…"), this)),
	statusProgress(new QProgressBar(this)),
	animeModel(new AnimeModel(control->animeModel(), this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	ui->setupUi(this);

	connect(control, &MainControl::showStatus,
			this, &MainWindow::showStatus);
	connect(control, &MainControl::setProgress,
			this, &MainWindow::setProgress);
	connect(control, &MainControl::reloadingAnimesChanged,
			this, &MainWindow::updateLoadStatus);

	connect(ui->actionReload_Seasons, &QAction::triggered,
			control, &MainControl::reload);
	connect(ui->action_Settings, &QAction::triggered,
			control, &MainControl::showSettings);
	connect(ui->action_About, &QAction::triggered,
			control, &MainControl::showAbout);
	connect(ui->actionAdd_Anime, &QAction::triggered,
			control, &MainControl::addAnime);
	connect(ui->actionPaste_ID_URL, &QAction::triggered,
			control, &MainControl::addAnimeFromClipboard);
	connect(ui->actionResync_Data, &QAction::triggered,
			control, &MainControl::resyncData);
	connect(ui->actionQuit, &QAction::triggered,
			this, &MainWindow::close);

	auto sep1 = new QAction(ui->seasonTreeView);
	sep1->setSeparator(true);
	auto sep2 = new QAction(ui->seasonTreeView);
	sep2->setSeparator(true);
	ui->seasonTreeView->addActions({
									   ui->actionCopy_selected_Info,
									   sep1,
									   ui->action_Open_Anime_in_browser,
									   ui->action_Unmark_new_seasons,
									   sep2,
									   ui->actionRemove_Anime
								   });

	statusLabel->hide();
	statusBar()->addWidget(statusLabel);
	statusProgress->setTextVisible(false);
	statusProgress->setFixedSize(125, 16);
	statusBar()->addPermanentWidget(statusProgress);
	statusProgress->hide();

	QSettings settings;
	settings.beginGroup(objectName());
	restoreGeometry(settings.value(QStringLiteral("geom")).toByteArray());
	restoreState(settings.value(QStringLiteral("state")).toByteArray());
	ui->seasonTreeView->header()->restoreState(settings.value(QStringLiteral("header")).toByteArray());
	settings.endGroup();

	proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	proxyModel->setSortLocaleAware(true);
	proxyModel->setSourceModel(animeModel);
	ui->seasonTreeView->setModel(proxyModel);

	connect(ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);
	connect(ui->actionAbout_Qt, &QAction::triggered,
			qApp, &QApplication::aboutQt);

	updateLoadStatus(control->isReloadingAnimes());
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.beginGroup(objectName());
	settings.setValue(QStringLiteral("geom"), saveGeometry());
	settings.setValue(QStringLiteral("state"), saveState());
	settings.setValue(QStringLiteral("header"), ui->seasonTreeView->header()->saveState());
	settings.endGroup();

	delete ui;
}

void MainWindow::showStatus(QString message)
{
	statusBar()->showMessage(message, 5000);
}

void MainWindow::setProgress(int value, int max)
{
	statusProgress->setMaximum(max);
	statusProgress->setValue(value);
}

void MainWindow::updateLoadStatus(bool isLoading)
{
	ui->actionReload_Seasons->setEnabled(!isLoading);
	ui->actionAdd_Anime->setEnabled(!isLoading);
	ui->actionRemove_Anime->setEnabled(!isLoading);
	ui->actionPaste_ID_URL->setEnabled(!isLoading);

	statusLabel->setVisible(isLoading);
	statusProgress->setRange(0, 0);
	statusProgress->setVisible(isLoading);
}

void MainWindow::addDockAction(QAction *action)
{
	if(!ui->menu_Actions->actions().contains(action))
		ui->menu_Actions->addAction(action);
}

void MainWindow::updatePreview(const QModelIndex &index)
{
	auto mIndex = mapToCtrl(index);
	if(mIndex.isValid()) {
		auto info = control->animeModel()->object(mIndex);
		control->showDetails(info->id());
		ui->action_Unmark_new_seasons->setEnabled(info->hasNewSeasons());
	} else
		control->showDetails(-1);
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	auto index = mapToCtrl(ui->seasonTreeView->currentIndex());
	if(index.isValid())
		control->removeAnime(control->animeModel()->object(index)->id());
}

void MainWindow::on_actionCopy_selected_Info_triggered()
{
	auto index = ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto clipBoard = QApplication::clipboard();

		auto rIndex = mapToCtrl(index);
		auto info = control->animeModel()->object(rIndex);
		switch (index.column()) {//columns are unchanged
		case 0:
			clipBoard->setText(QString::number(info->id()));
			showStatus(tr("Copied Anime Id: %1").arg(info->id()));
			break;
		case 1:
			clipBoard->setText(info->title());
			showStatus(tr("Copied Anime Title: %1").arg(info->title()));
			break;
		case 2:
			clipBoard->setText(QLocale().toString(info->totalSeasonCount()));
			showStatus(tr("Copied Season Count: %1").arg(info->totalSeasonCount()));
			break;
		case 3:
			clipBoard->setText(info->relationsUrl().toString());
			showStatus(tr("Copied Relations URL: %1").arg(info->relationsUrl().toString()));
			break;
		default:
			break;
		}
	}
}

void MainWindow::on_action_Open_Anime_in_browser_triggered()
{
	auto index = mapToCtrl(ui->seasonTreeView->currentIndex());
	if(index.isValid())
		control->animeModel()->object(index)->openUrl();
}

void MainWindow::on_action_Unmark_new_seasons_triggered()
{
	auto rIndex = mapToCtrl(ui->seasonTreeView->currentIndex());
	if(rIndex.isValid())
		control->uncheckAnime(control->animeModel()->object(rIndex)->id());
}

void MainWindow::on_seasonTreeView_activated(const QModelIndex &index)
{
	auto rIndex = mapToCtrl(index);
	if(rIndex.isValid())
		control->itemAction(control->animeModel()->object(rIndex)->id());
}

QModelIndex MainWindow::mapToCtrl(const QModelIndex &uiIndex) const
{
	if(uiIndex.isValid()) {
		auto pIndex = proxyModel->mapToSource(uiIndex);
		if(pIndex.isValid())
			return animeModel->mapToSource(pIndex);
	}

	return {};
}
