#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "animemodel.h"

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QSortFilterProxyModel>

#include <maincontrol.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	Q_INVOKABLE MainWindow(Control *mControl, QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void showStatus(QString message);
	void setProgress(int value, int max);

	void updateLoadStatus(bool isLoading);

private slots:
	void updatePreview(const QModelIndex &index);

	void on_actionRemove_Anime_triggered();
	void on_actionCopy_selected_Info_triggered();
	void on_seasonTreeView_activated(const QModelIndex &index);
	void on_action_About_triggered();

private:
	MainControl *control;

	Ui::MainWindow *ui;
	QLabel *statusLabel;
	QProgressBar *statusProgress;

	AnimeModel *animeModel;
	QSortFilterProxyModel *proxyModel;

	QModelIndex mapToCtrl(const QModelIndex &uiIndex) const;
};

#endif // MAINWINDOW_H
