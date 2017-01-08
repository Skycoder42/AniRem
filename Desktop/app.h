#ifndef APP_H
#define APP_H

#include "mainwindow.h"

#include <QApplication>

#include <animestore.h>

class App : public QApplication
{
	Q_OBJECT

public:
	explicit App(int &argc, char **argv);
	~App();

	int exec();

private slots:
	void showError(QString title, QString message);

	void reload();
	void storeLoaded();

private:
	AnimeStore *store;
	MainWindow *mainWindow;

	void init();
};

#endif // APP_H
