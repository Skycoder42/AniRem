#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addanimedialog.h"
#include <dialogmaster.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	model(new AnimeSeasonModel(this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	this->ui->setupUi(this);

	connect(this->model, &AnimeSeasonModel::modelError,
			this, &MainWindow::modelError);

	this->proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	this->proxyModel->setSortLocaleAware(true);
	this->proxyModel->setSourceModel(this->model);
	this->ui->seasonTreeView->setModel(this->proxyModel);
}

MainWindow::~MainWindow()
{
	delete this->ui;
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
