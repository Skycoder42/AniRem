#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addanimedialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	model(new AnimeSeasonModel(this)),
	proxyModel(new QSortFilterProxyModel(this))
{
	this->ui->setupUi(this);

	this->proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	this->proxyModel->setSortLocaleAware(true);
	this->proxyModel->setSourceModel(this->model);
	this->ui->seasonTreeView->setModel(this->proxyModel);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::on_actionAdd_Anime_triggered()
{
	auto info = AddAnimeDialog::createInfo(-1, this);
	if(info.id != -1) {

	}
}
