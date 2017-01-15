#include "maincontrol.h"

MainControl::MainControl(AnimeStore *store, QObject *parent) :
	Control(parent)
{}

void MainControl::reload()
{
	emit updateLoadStatus(false);
	emit showStatus(tr("Checking for new seasons…"), true);

	//TODO perform loading
}

void MainControl::addAnime()
{
	Q_UNIMPLEMENTED();
}

void MainControl::addAnimeFromClipboard()
{
	Q_UNIMPLEMENTED();
}

void MainControl::showDetails(int id)
{
	Q_UNIMPLEMENTED();
}

void MainControl::onShow()
{
	emit updateLoadStatus(false);
}
