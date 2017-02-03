#ifndef PROXERAPP_H
#define PROXERAPP_H

#include "animestore.h"
#include "seasonstatusloader.h"
#include "maincontrol.h"
#include "statuscontrol.h"

#include <coreapp.h>

class ProxerApp : public CoreApp
{
	Q_OBJECT

public:
	explicit ProxerApp(QObject *parent = nullptr);

public slots:
	void checkForSeasonUpdate(AnimeInfo *animeInfo);
	void checkForSeasonUpdates();
	void showMainControl();

	void quitApp();

protected:
	void setupParser(QCommandLineParser &parser, bool &allowInvalid) const override;
	bool startApp(const QCommandLineParser &parser) override;

protected slots:
	void aboutToQuit() override;

private slots:
	void storeLoaded();
	void updateDone(bool hasUpdates, QString errorString);

private:
	AnimeStore *store;
	SeasonStatusLoader *loader;
	MainControl *mainControl;
	StatusControl *statusControl;

	bool passiveUpdate;
	bool showNoUpdatesInfo;

	void automaticUpdateCheck();
};

#undef coreApp
#define coreApp static_cast<ProxerApp*>(CoreApp::instance())

#endif // PROXERAPP_H