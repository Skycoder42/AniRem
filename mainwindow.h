#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include "animeseasonmodel.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void updatePreview(const QModelIndex &index);
	void modelError(QString error);

	void on_actionAdd_Anime_triggered();

private:
	Ui::MainWindow *ui;

	AnimeSeasonModel *model;
	QSortFilterProxyModel *proxyModel;
};

#endif // MAINWINDOW_H
