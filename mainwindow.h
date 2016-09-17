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
	explicit MainWindow(AnimeStore *store, QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void open();
	void showStatus(QString message);

	void loadingCompleted(const QList<AnimeInfo> &animeInfos, bool canEdit);

private slots:
	void updatePreview(const QModelIndex &index);

	void on_actionAdd_Anime_triggered();
	void on_actionRemove_Anime_triggered();
	void on_actionPaste_ID_URL_triggered();
	void on_actionCopy_selected_Info_triggered();

	void on_seasonTreeView_activated(const QModelIndex &index);

private:
	Ui::MainWindow *ui;

	AnimeSeasonModel *model;
	QSortFilterProxyModel *proxyModel;
};

#endif // MAINWINDOW_H
