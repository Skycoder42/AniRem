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
	statusProgress(new QProgressBar(this)),
	animeModel(new AnimeModel(control->animeModel(), this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	ui->setupUi(this);
	ui->menuView->addAction(ui->previewDock->toggleViewAction());

	connect(control, &MainControl::showStatus,
			this, &MainWindow::showStatus);
	connect(control, &MainControl::setProgress,
			this, &MainWindow::setProgress);
	connect(control, &MainControl::updateLoadStatus,
			this, &MainWindow::updateLoadStatus);

	connect(ui->actionReload_Seasons, &QAction::triggered,
			control, &MainControl::reload);
	connect(ui->actionAdd_Anime, &QAction::triggered,
			control, &MainControl::addAnime);
	connect(ui->actionPaste_ID_URL, &QAction::triggered,
			control, &MainControl::addAnimeFromClipboard);
	connect(ui->actionQuit, &QAction::triggered,
			this, &MainWindow::close);

	ui->seasonTreeView->addActions({
									   ui->actionRemove_Anime,
									   ui->actionCopy_selected_Info
								   });

	statusProgress->setTextVisible(false);
	statusProgress->setFixedSize(125, 16);
	statusBar()->addPermanentWidget(statusProgress);
	statusProgress->hide();

	QSettings settings;
	settings.beginGroup(objectName());
	restoreGeometry(settings.value(QStringLiteral("geom")).toByteArray());
	restoreState(settings.value(QStringLiteral("state")).toByteArray());
	restoreDockWidget(ui->previewDock);
	ui->seasonTreeView->header()->restoreState(settings.value(QStringLiteral("header")).toByteArray());
	settings.endGroup();

	proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	proxyModel->setSortLocaleAware(true);
	proxyModel->setSourceModel(animeModel);
	//ui->seasonTreeView->setModel(proxyModel);
	ui->seasonTreeView->setModel(animeModel);
	//ui->seasonTreeView->setModel(control->animeModel());

	connect(ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);
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

void MainWindow::showStatus(QString message, bool permanent)
{
	statusBar()->showMessage(message, permanent ? 0 : 5000);
}

void MainWindow::setProgress(int value, int max)
{
	statusProgress->setMaximum(max);
	statusProgress->setValue(value);
}

void MainWindow::updateLoadStatus(bool isFinished)
{
	ui->actionReload_Seasons->setEnabled(isFinished);
	ui->actionAdd_Anime->setEnabled(isFinished);
	ui->actionRemove_Anime->setEnabled(isFinished);
	ui->actionPaste_ID_URL->setEnabled(isFinished);

	statusProgress->setRange(0, 0);
	statusProgress->setVisible(!isFinished);
	if(isFinished)
		statusBar()->clearMessage();
}

void MainWindow::updatePreview(const QModelIndex &index)
{
	auto mIndex = mapToCtrl(index);
	if(mIndex.isValid()) {
		auto info = control->animeModel()->object(mIndex);
		ui->dockWidgetContents->setText("<i>Loading preview image&#8230;</i>");
		ImageLoader::instance()->loadImage(info->id(), [this](int id, QPixmap pm){
			auto index = mapToCtrl(ui->seasonTreeView->currentIndex());
			if(index.isValid()) {
				auto info = control->animeModel()->object(index);
				if(info->id() == id)
					ui->dockWidgetContents->setPixmap(pm);
			}
		});
	} else
		ui->dockWidgetContents->clear();
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	//DEBUG
	qDebug() << control->animeModel()->rowCount()
			 << animeModel->rowCount()
			 << proxyModel->rowCount();
	qDebug() << animeModel->columnCount({})
			 << proxyModel->columnCount({});
	qDebug() << control->animeModel()->object(0)->title();
	qDebug() << control->animeModel()->data(control->animeModel()->index(0, 0),
											control->animeModel()->roleNames().key("title"));
	qDebug() << animeModel->data(animeModel->index(0, 0), Qt::DisplayRole);
	qDebug() << proxyModel->data(proxyModel->index(0, 0), Qt::DisplayRole);
	return;


	auto index = mapToCtrl(ui->seasonTreeView->currentIndex());
	if(index.isValid())
		control->removeAnime(index);
}

void MainWindow::on_actionCopy_selected_Info_triggered()//TODO move to core?
{
	auto index = ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto clipBoard = QApplication::clipboard();

		if(ui->dockWidgetContents->hasFocus()) {
			clipBoard->setImage(ui->dockWidgetContents->pixmap()->toImage());
			showStatus(tr("Copied Preview image"));
		} else {
			auto rIndex = mapToCtrl(index);
			auto info = control->animeModel()->object(rIndex);
			switch (rIndex.column()) {
			case 0:
				clipBoard->setText(info->title());
				showStatus(tr("Copied Anime Title: %1").arg(info->title()));
				break;
			case 1:
				clipBoard->setText(QLocale().toString(info->lastKnownSeasons()));
				showStatus(tr("Copied Season Count: %1").arg(info->lastKnownSeasons()));
				break;
			case 2:
				clipBoard->setText(info->relationsUrl().toString());
				showStatus(tr("Copied Relations URL: %1").arg(info->relationsUrl().toString()));
				break;
			default:
				break;
			}
		}
	}
}

void MainWindow::on_seasonTreeView_activated(const QModelIndex &index)
{
	auto rIndex = mapToCtrl(index);
	if(rIndex.isValid())
		control->uncheckAnime(rIndex);
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
