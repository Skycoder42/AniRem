#ifndef PROXERAPP_H
#define PROXERAPP_H

#include "animestore.h"
#include "seasonstatusloader.h"
#include "maincontrol.h"

#include <coreapp.h>

class ProxerApp : public CoreApp
{
	Q_OBJECT

public:
	explicit ProxerApp(QObject *parent = nullptr);

public slots:
	void checkForSeasonUpdates();

protected:
	void setupParser(QCommandLineParser &parser, bool &allowInvalid) override;
	bool startApp(const QCommandLineParser &parser) override;

protected slots:
	void aboutToQuit() override;

private slots:
	void storeLoaded(bool loading);
	void updateDone(QString errorString);

private:
	AnimeStore *store;
	SeasonStatusLoader *loader;
	MainControl *mainControl;
};

#undef coreApp
#define coreApp static_cast<ProxerApp*>(CoreApp::instance())

#endif // PROXERAPP_H
