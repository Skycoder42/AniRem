#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QtMvvmCore/Binding>
#include <localsettings.h>
#include "instancesetup.h"

#ifndef NO_AUTO_UPDATER
#include <QtAutoUpdaterGui/UpdateController>
#endif

MainWindow::MainWindow(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QMainWindow(parent),
	_viewModel(static_cast<MainViewModel*>(viewModel)),
	_ui(new Ui::MainWindow),
	_statusLabel(new QLabel(this)),
	_statusProgress(new QProgressBar(this)),
	_animeModel(new AnimeModel(_viewModel->animeModel(), this)),
	_proxyModel(new QSortFilterProxyModel(this))
{
	_ui->setupUi(this);

	connect(_viewModel, &MainViewModel::showStatus,
			this, &MainWindow::showStatus);
	connect(_viewModel, &MainViewModel::setProgress,
			this, &MainWindow::setProgress);
	connect(_viewModel, &MainViewModel::reloadingAnimesChanged,
			this, &MainWindow::updateLoadStatus);

	connect(_ui->actionReload_Seasons, &QAction::triggered,
			_viewModel, &MainViewModel::reload);
	connect(_ui->action_Settings, &QAction::triggered,
			_viewModel, &MainViewModel::showSettings);
	connect(_ui->action_About, &QAction::triggered,
			_viewModel, &MainViewModel::showAbout);
	connect(_ui->action_Unblock_IP_Captcha, &QAction::triggered,
			_viewModel, &MainViewModel::showCaptcha);
	connect(_ui->actionAdd_Anime, &QAction::triggered,
			_viewModel, &MainViewModel::addAnime);
	connect(_ui->actionPaste_ID_URL, &QAction::triggered,
			_viewModel, &MainViewModel::addAnimeFromClipboard);
	connect(_ui->actionSynchronization, &QAction::triggered,
			_viewModel, &MainViewModel::showSync);
	connect(_ui->actionQuit, &QAction::triggered,
			this, &MainWindow::close);

	auto sep1 = new QAction(_ui->seasonTreeView);
	sep1->setSeparator(true);
	auto sep2 = new QAction(_ui->seasonTreeView);
	sep2->setSeparator(true);
	_ui->seasonTreeView->addActions({
									   _ui->actionCopy_selected_Info,
									   sep1,
									   _ui->action_Open_Anime_in_browser,
									   _ui->action_Unmark_new_seasons,
									   sep2,
									   _ui->actionRemove_Anime
								   });

	statusBar()->addWidget(_statusLabel);
	_statusProgress->setTextVisible(false);
	_statusProgress->setFixedSize(125, 16);
	statusBar()->addPermanentWidget(_statusProgress);
	_statusProgress->hide();

	_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	_proxyModel->setSortLocaleAware(true);
	_proxyModel->setSourceModel(_animeModel);
	_ui->seasonTreeView->setModel(_proxyModel);

	connect(_viewModel->animeModel(), &QObjectListModel::rowsInserted,
			this, &MainWindow::updateCount);
	connect(_viewModel->animeModel(), &QObjectListModel::rowsRemoved,
			this, &MainWindow::updateCount);
	connect(_viewModel->animeModel(), &QObjectListModel::modelReset,
			this, &MainWindow::updateCount);
	updateCount();

	connect(_ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);

	updateLoadStatus(_viewModel->isReloadingAnimes());

#ifndef NO_AUTO_UPDATER
	auto updater = new QtAutoUpdater::UpdateController(this, qApp);
	auto action = _ui->menu_Help->actions().first();
	_ui->menu_Help->insertAction(action, updater->createUpdateAction(_ui->menu_Help));
	_ui->menu_Help->insertSeparator(action);
	updater->start();
#endif

	//restore window geom
	if(LocalSettings::instance()->gui.mainwindow.geom.isSet())
		restoreGeometry(LocalSettings::instance()->gui.mainwindow.geom);
	if(LocalSettings::instance()->gui.mainwindow.state.isSet())
		restoreState(LocalSettings::instance()->gui.mainwindow.state);
	if(LocalSettings::instance()->gui.mainwindow.header.isSet())
		_ui->seasonTreeView->header()->restoreState(LocalSettings::instance()->gui.mainwindow.header);

	// final step: register in app
	setMainWindow(this);
}

MainWindow::~MainWindow()
{
	LocalSettings::instance()->gui.mainwindow.geom = saveGeometry();
	LocalSettings::instance()->gui.mainwindow.state = saveState();
	LocalSettings::instance()->gui.mainwindow.header = _ui->seasonTreeView->header()->saveState();

	delete _ui;
}

void MainWindow::showStatus(const QString &message)
{
	statusBar()->showMessage(message, 5000);
}

void MainWindow::setProgress(int value, int max)
{
	_statusProgress->setMaximum(max);
	_statusProgress->setValue(value);
}

void MainWindow::updateLoadStatus(bool isLoading)
{
	_ui->actionReload_Seasons->setEnabled(!isLoading);
	_ui->actionAdd_Anime->setEnabled(!isLoading);
	_ui->actionRemove_Anime->setEnabled(!isLoading);
	_ui->actionPaste_ID_URL->setEnabled(!isLoading);

	if(isLoading)
		statusBar()->showMessage(tr("Checking for new seasons…"));
	else
		statusBar()->clearMessage();
	_statusProgress->setRange(0, 0);
	_statusProgress->setVisible(isLoading);
}

void MainWindow::addDockAction(QAction *action)
{
	if(!_ui->menu_Actions->actions().contains(action))
		_ui->menu_Actions->addAction(action);
}

void MainWindow::updatePreview(const QModelIndex &index)
{
	auto mIndex = mapToModel(index);
	if(mIndex.isValid()) {
		auto info = _viewModel->animeModel()->object<AnimeInfo>(mIndex);
		_viewModel->showDetails(info.id());
		_ui->action_Unmark_new_seasons->setEnabled(info.hasNewSeasons());
	} else
		_viewModel->showDetails(-1);
}

void MainWindow::updateCount()
{
	_statusLabel->setText(tr("Animes in list: %L1").arg(_viewModel->animeModel()->rowCount()));
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	auto id = infoId(_ui->seasonTreeView->currentIndex());
	if(id >= 0)
		_viewModel->removeAnime(id);

}

void MainWindow::on_actionCopy_selected_Info_triggered()
{
	auto index = _ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto clipBoard = QApplication::clipboard();

		auto rIndex = mapToModel(index);
		auto info = _viewModel->animeModel()->object<AnimeInfo>(rIndex);
		switch (index.column()) {//columns are unchanged
		case 0:
			clipBoard->setText(QString::number(info.id()));
			showStatus(tr("Copied Anime Id: %1").arg(info.id()));
			break;
		case 1:
			clipBoard->setText(info.title());
			showStatus(tr("Copied Anime Title: %1").arg(info.title()));
			break;
		case 2:
			clipBoard->setText(QLocale().toString(info.totalSeasonCount()));
			showStatus(tr("Copied Season Count: %L1").arg(info.totalSeasonCount()));
			break;
		case 3:
			clipBoard->setText(QLocale().toString(info.lastUpdateCheck()));
			showStatus(tr("Copied Last updated: %1").arg(clipBoard->text()));
			break;
		case 4:
			clipBoard->setText(info.relationsUrl().toString());
			showStatus(tr("Copied Relations URL: %1").arg(info.relationsUrl().toString()));
			break;
		default:
			break;
		}
	}
}

void MainWindow::on_action_Open_Anime_in_browser_triggered()
{
	auto id = infoId(_ui->seasonTreeView->currentIndex());
	if(id >= 0)
		_viewModel->openUrl(id);
}

void MainWindow::on_action_Unmark_new_seasons_triggered()
{
	auto id = infoId(_ui->seasonTreeView->currentIndex());
	if(id >= 0)
		_viewModel->uncheckAnime(id);
}

void MainWindow::on_seasonTreeView_activated(const QModelIndex &index)
{
	auto id = infoId(index);
	if(id >= 0)
		_viewModel->itemAction(id);
}

void MainWindow::on_actionMigrate_from_SeasonProxer_triggered()
{
	//TODO implement in core to migrate datasync
}

QModelIndex MainWindow::mapToModel(const QModelIndex &uiIndex) const
{
	auto pIndex = _proxyModel->mapToSource(uiIndex);
	return _animeModel->mapToSource(pIndex);
}

int MainWindow::infoId(const QModelIndex &uiIndex) const
{
	auto index = mapToModel(uiIndex);
	if(index.isValid())
		return _viewModel->animeModel()->key<int>(index);
	else
		return -1;
}
