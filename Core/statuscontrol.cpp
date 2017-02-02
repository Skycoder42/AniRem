#include "statuscontrol.h"
#include "proxerapp.h"

StatusControl::StatusControl(QObject *parent) :
	Control(parent)
{}

void StatusControl::loadUpdateStatus(AnimeList animes)
{
	QStringList updatesList;
	foreach (auto anime, animes) {
		if(anime->hasNewSeasons()) {
			updatesList.append(tr("• %1: %n season(s)", "", anime->totalSeasonCount())
							   .arg(anime->title()));
		}
	}

	emit showUpdateNotification(true,
								tr("%n new season(s) detected!", "", updatesList.size()),
								updatesList.join(QLatin1Char('\n')));
}

void StatusControl::loadErrorStatus(const QString &error)
{
	emit showUpdateNotification(false, tr("Season check failed!"), error);
}

void StatusControl::showMainControl()
{
	coreApp->showMainControl();
}
