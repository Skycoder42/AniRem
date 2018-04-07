#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QSortFilterProxyModel>

#include <mainviewmodel.h>

#include "animemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	Q_INVOKABLE MainWindow(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~MainWindow() override;

public slots:
	void showStatus(const QString &message);
	void setProgress(int value, int max);

	void updateLoadStatus(bool isLoading);

	void addDockAction(QAction *action);

private slots:
	void updatePreview(const QModelIndex &index);
	void updateCount();

	void on_actionRemove_Anime_triggered();
	void on_actionCopy_selected_Info_triggered();
	void on_action_Open_Anime_in_browser_triggered();
	void on_action_Unmark_new_seasons_triggered();
	void on_seasonTreeView_activated(const QModelIndex &index);

private:
	MainViewModel *_viewModel;
	Ui::MainWindow *_ui;

	QLabel *_statusLabel;
	QProgressBar *_statusProgress;

	AnimeModel *_animeModel;
	QSortFilterProxyModel *_proxyModel;

	QModelIndex mapToModel(const QModelIndex &uiIndex) const;
	int infoId(const QModelIndex &uiIndex) const;
};

#endif // MAINWINDOW_H
