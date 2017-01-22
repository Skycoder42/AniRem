#include "statuscontrol.h"
#include "proxerapp.h"

StatusControl::StatusControl(QObject *parent) :
	Control(parent),
	success(false),
	title(),
	message()
{}

void StatusControl::loadUpdateStatus(AnimeList animes)
{
	QStringList updatesList;
	foreach (auto anime, animes) {
		if(anime->hasNewSeasons()) {
			updatesList.append(tr(" • %1: %n season(s)\n", "", anime->lastKnownSeasons())
							   .arg(anime->title()));
		}
	}

	success = true;
	title = tr("%n new season(s) detected!", "", updatesList.size());
	message = updatesList.join(QLatin1Char('\n'));
}

void StatusControl::loadErrorStatus(const QString &error)
{
	success = false;
	title = tr("Season check failed!");
	message = error;
}

void StatusControl::onShow()
{
	emit showUpdateNotification(success, title, message);
}

void StatusControl::showMainControl()
{
	coreApp->showMainControl();
}
