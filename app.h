#ifndef APP_H
#define APP_H

#include <QApplication>
#include "mainwindow.h"

class App : public QApplication
{
	Q_OBJECT
public:
	explicit App(int &argc, char **argv);
	~App();

	int exec();

private:
	MainWindow *mainWindow;
};

#endif // APP_H
