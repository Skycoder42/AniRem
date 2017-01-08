#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QSettings>
#include "addanimedialog.h"
#include "app.h"

MainWindow::MainWindow(AnimeStore *store, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	statusProgress(new QProgressBar(this)),
	model(new AnimeModel(store, this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	ui->setupUi(this);	
	ui->menuView->addAction(ui->previewDock->toggleViewAction());

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
	proxyModel->setSourceModel(model);
	ui->seasonTreeView->setModel(proxyModel);

	connect(ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);
	connect(ui->actionReload_Seasons, &QAction::triggered,
			this, &MainWindow::reload);

	updateLoadStatus(false);
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

void MainWindow::open()
{
	show();
	raise();
	setFocus();
	activateWindow();
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
	auto info = model->animeInfo(proxyModel->mapToSource(index));
	ui->dockWidgetContents->setText("<i>Loading preview image&#8230;</i>");
	qApp->imageLoader()->loadImage(info->id(), [this](int id, QPixmap pm){
		auto info = model->animeInfo(proxyModel->mapToSource(ui->seasonTreeView->currentIndex()));
		if(info->id() == id)
			ui->dockWidgetContents->setPixmap(pm);
	});
}

void MainWindow::on_actionAdd_Anime_triggered()
{
	auto info = AddAnimeDialog::createInfo(-1, this);
	if(info) {
		model->addAnime(info);
		showStatus(tr("Added Anime: %1").arg(info->title()));
	}
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	auto index = ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto info = model->removeInfo(proxyModel->mapToSource(index));
		showStatus(tr("Removed Anime: %1").arg(info->title()));
	}
}

void MainWindow::on_actionPaste_ID_URL_triggered()
{
	auto clipBoard = QApplication::clipboard();
	auto text = clipBoard->text();

	auto id = -1;
	auto url = QUrl::fromUserInput(text);
	if(url.isValid() && !url.isRelative()) {
		if(url.host() == QStringLiteral("proxer.me")) {
			QStringList pathElements = url.path().split("/", QString::SkipEmptyParts);
			if(pathElements.size() >= 2) {
				bool ok = false;
				id = pathElements[1].toInt(&ok);
				if(!ok)
					id = -1;
			}
		}
	}

	if(id == -1) {
		bool ok = false;
		id = text.toInt(&ok);
		if(!ok)
			id = -1;
	}

	if(id != -1) {
		auto info = AddAnimeDialog::createInfo(id, this);
		if(info){
			model->addAnime(info);
			showStatus(tr("Added Anime: %1").arg(info->title()));
		}
	}
}

void MainWindow::on_actionCopy_selected_Info_triggered()
{
	auto index = ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto clipBoard = QApplication::clipboard();

		if(ui->dockWidgetContents->hasFocus()) {
			clipBoard->setImage(ui->dockWidgetContents->pixmap()->toImage());
			showStatus(tr("Copied Preview image"));
		} else {
			auto rIndex = proxyModel->mapToSource(index);
			auto info = model->animeInfo(rIndex);
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
	if(index.isValid()) {
		auto info = model->animeInfo(proxyModel->mapToSource(index));
		model->uncheckAnime(index);
		QDesktopServices::openUrl(info->relationsUrl());
	}
}
