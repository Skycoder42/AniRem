#include "detailscontrol.h"

DetailsControl::DetailsControl(QObject *parent) :
	Control(parent),
	_animeInfo(nullptr)
{}

AnimeInfo *DetailsControl::animeInfo() const
{
	return _animeInfo;
}

QString DetailsControl::detailsText() const
{
	auto string = QStringLiteral("<table>");
	for(auto it = _animeInfo->seasonState().constBegin(); it != _animeInfo->seasonState().constEnd(); it++) {
		auto row = tr("<tr><td>&nbsp;%2:&nbsp;</td><td>&nbsp;%L1&nbsp;</td></tr>");
		if(it->second) {
			row = row.arg(tr("<b><font color=\"#8A0E0E\">%L1 (new)</font></b>"));
		}
		string += row.arg(it->first)
				  .arg(_animeInfo->typeToString(it.key()));
	}
	return string + QStringLiteral("</table>");
}

void DetailsControl::setAnimeInfo(AnimeInfo *animeInfo)
{
	if (_animeInfo == animeInfo)
		return;

	disconnect(_animeInfo, &AnimeInfo::seasonStateChanged,
			   this, &DetailsControl::animeInfoChanged);
	_animeInfo = animeInfo;
	connect(_animeInfo, &AnimeInfo::seasonStateChanged,
			this, &DetailsControl::animeInfoChanged);
	emit animeInfoChanged();
}
