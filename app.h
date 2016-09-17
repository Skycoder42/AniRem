#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QSystemTrayIcon>
#include "animestore.h"
#include "mainwindow.h"
#include "seasonstatusloader.h"

class App : public QApplication
{
	Q_OBJECT
public:
	explicit App(int &argc, char **argv);
	~App();

	int exec();

private slots:
	void showError(QString title, QString message);
	void trayActivated();

	void reload();
	void storeLoaded();
	void seasonsLoaded(bool hasNew);

private:
	AnimeStore *store;
	SeasonStatusLoader *loader;
	bool isUpdateMode;

	MainWindow *mainWindow;

	QSystemTrayIcon *trayIco;
	bool trayHigh;

	void init();
	void initTray();
};

#endif // APP_H
