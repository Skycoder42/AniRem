#ifndef APP_H
#define APP_H

#include "imageloader.h"
#include "mainwindow.h"

#include <QApplication>

#include <animestore.h>

class App : public QApplication
{
	Q_OBJECT

public:
	explicit App(int &argc, char **argv);
	~App();

	ImageLoader *imageLoader() const;

	int exec();

private slots:
	void showError(QString title, QString message);

	void reload();
	void storeLoaded();

private:
	AnimeStore *store;
	ImageLoader *imgLoader;
	MainWindow *mainWindow;

	void init();
};

#undef qApp
#define qApp static_cast<App*>(QApplication::instance())

#endif // APP_H
