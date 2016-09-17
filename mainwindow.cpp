#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addanimedialog.h"
#include <dialogmaster.h>
#include <QSettings>
#include <QClipboard>
#include <QMimeData>
#include <QStringList>
#include <QDesktopServices>
#include <QProgressBar>

MainWindow::MainWindow(AnimeStore *store, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	model(new AnimeSeasonModel(store, this)),//TODO!!!
	proxyModel(new QSortFilterProxyModel(this))
{
	this->ui->setupUi(this);
	this->ui->menuView->addAction(this->ui->previewDock->toggleViewAction());

	this->ui->seasonTreeView->addActions({
											 this->ui->actionRemove_Anime,
											 this->ui->actionCopy_selected_Info
										 });

	QSettings settings;
	settings.beginGroup(this->objectName());
	this->restoreGeometry(settings.value(QStringLiteral("geom")).toByteArray());
	this->restoreState(settings.value(QStringLiteral("state")).toByteArray());
	this->restoreDockWidget(this->ui->previewDock);
	this->ui->seasonTreeView->header()->restoreState(settings.value(QStringLiteral("header")).toByteArray());
	settings.endGroup();

	this->proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	this->proxyModel->setSortLocaleAware(true);
	this->proxyModel->setSourceModel(this->model);
	this->ui->seasonTreeView->setModel(this->proxyModel);

	connect(this->ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);

	this->loadingCompleted({}, false);
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.beginGroup(this->objectName());
	settings.setValue(QStringLiteral("geom"), this->saveGeometry());
	settings.setValue(QStringLiteral("state"), this->saveState());
	settings.setValue(QStringLiteral("header"), this->ui->seasonTreeView->header()->saveState());
	settings.endGroup();

	delete this->ui;
}

void MainWindow::open()
{
	this->show();
	this->raise();
	this->setFocus();
	this->activateWindow();
}

void MainWindow::showStatus(QString message)
{
	this->statusBar()->showMessage(message, 5000);
}

void MainWindow::loadingCompleted(const QList<AnimeInfo> &animeInfos, bool canEdit)
{
	this->model->setAnimeList(animeInfos);
	this->ui->actionReload_Seasons->setEnabled(canEdit);
	this->ui->actionAdd_Anime->setEnabled(canEdit);
	this->ui->actionRemove_Anime->setEnabled(canEdit);
	this->ui->actionPaste_ID_URL->setEnabled(canEdit);
}

void MainWindow::updatePreview(const QModelIndex &index)
{
	auto info = this->model->animeInfo(this->proxyModel->mapToSource(index));
	this->ui->dockWidgetContents->setPixmap(info.previewImage);
}

void MainWindow::on_actionAdd_Anime_triggered()
{
	auto info = AddAnimeDialog::createInfo(-1, this);
	if(info.id != -1) {
		this->model->addAnime(info);
		this->showStatus(tr("Added Anime: %1").arg(info.title));
	}
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	auto index = this->ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto info = this->model->removeInfo(this->proxyModel->mapToSource(index));
		this->showStatus(tr("Removed Anime: %1").arg(info.title));
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
		if(info.id != -1)
			this->model->addAnime(info);
	}
}

void MainWindow::on_actionCopy_selected_Info_triggered()
{
	auto index = this->ui->seasonTreeView->currentIndex();
	if(index.isValid()) {
		auto clipBoard = QApplication::clipboard();

		if(this->ui->dockWidgetContents->hasFocus()) {
			clipBoard->setImage(this->ui->dockWidgetContents->pixmap()->toImage());
			this->showStatus(tr("Copied Preview image"));
		} else {
			auto rIndex = this->proxyModel->mapToSource(index);
			auto info = this->model->animeInfo(rIndex);
			switch (rIndex.column()) {
			case 0:
				clipBoard->setText(info.title);
				this->showStatus(tr("Copied Anime Title: %1").arg(info.title));
				break;
			case 1:
				clipBoard->setText(QLocale().toString(info.lastKnownSeasons));
				this->showStatus(tr("Copied Season Count: %1").arg(info.lastKnownSeasons));
				break;
			case 2:
				clipBoard->setText(info.relationsUrl().toString());
				this->showStatus(tr("Copied Relations URL: %1").arg(info.relationsUrl().toString()));
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
		auto info = this->model->animeInfo(this->proxyModel->mapToSource(index));
		this->model->uncheckAnime(index);
		QDesktopServices::openUrl(info.relationsUrl());
	}
}
