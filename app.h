#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QSystemTrayIcon>
#include "animestore.h"
#include "mainwindow.h"

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

	void storeLoaded(QList<AnimeInfo> infoList);

private:
	AnimeStore *store;
	bool isUpdateMode;

	QSystemTrayIcon *trayIco;
	MainWindow *mainWindow;

	void init();
	void initTray();
};

#endif // APP_H
