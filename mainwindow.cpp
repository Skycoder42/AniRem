#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addanimedialog.h"
#include <dialogmaster.h>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	model(new AnimeSeasonModel(this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	this->ui->setupUi(this);
	this->ui->menuView->addAction(this->ui->previewDock->toggleViewAction());

	QSettings settings;
	settings.beginGroup(this->objectName());
	this->restoreGeometry(settings.value(QStringLiteral("geom")).toByteArray());
	this->restoreState(settings.value(QStringLiteral("state")).toByteArray());
	this->restoreDockWidget(this->ui->previewDock);
	this->ui->seasonTreeView->header()->restoreState(settings.value(QStringLiteral("header")).toByteArray());
	settings.endGroup();

	connect(this->model, &AnimeSeasonModel::modelError,
			this, &MainWindow::modelError);

	this->proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	this->proxyModel->setSortLocaleAware(true);
	this->proxyModel->setSourceModel(this->model);
	this->ui->seasonTreeView->setModel(this->proxyModel);

	connect(this->ui->seasonTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
			this, &MainWindow::updatePreview);
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

void MainWindow::updatePreview(const QModelIndex &index)
{
	auto info = this->model->animeInfo(this->proxyModel->mapToSource(index));
	this->ui->dockWidgetContents->setPixmap(info.previewImage);
}

void MainWindow::modelError(QString error)
{
	DialogMaster::critical(this, error, tr("Data-Model-Error"));
}

void MainWindow::on_actionAdd_Anime_triggered()
{
	auto info = AddAnimeDialog::createInfo(-1, this);
	if(info.id != -1) {
		this->model->addAnime(info);
	}
}

void MainWindow::on_actionRemove_Anime_triggered()
{
	this->model->removeInfo(this->proxyModel->mapToSource(this->ui->seasonTreeView->currentIndex()));
}
