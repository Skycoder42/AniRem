#include "animemodel.h"

#include <QFont>
#include <QLocale>

AnimeModel::AnimeModel(GenericListModel<AnimeInfo> *srcModel, QObject *parent) :
	ObjectProxyModel({tr("Name"), tr("Season Count"), tr("Season Overview")}, parent)
{
	setSourceModel(srcModel);
	addMapping(0, Qt::DisplayRole, "title");
	addMapping(1, Qt::DisplayRole, "lastKnownSeasons");
	addMapping(2, Qt::DisplayRole, "relationsUrl");
}

QVariant AnimeModel::data(const QModelIndex &index, int role) const
{
	auto src = mapToSource(index);
	if(!src.isValid())
		return {};

	auto info = qobject_cast<AnimeInfo*>(sourceModel()->object(src));
	switch (role) {
	case Qt::DisplayRole:
		switch(index.column()) {
		case 1:
			return info->lastKnownSeasons() < 0 ?
					tr("Not loaded") :
					QLocale().toString(info->lastKnownSeasons());
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(info->hasNewSeasons()) {
			QFont font;
			font.setBold(true);
			return font;
		} else if(index.column() == 1 &&
				  info->lastKnownSeasons() < 0) {
			QFont font;
			font.setItalic(true);
			return font;
		} else
			break;
	case Qt::ForegroundRole:
		if(info->hasNewSeasons())
			return QColor(Qt::red);
		else
			break;
	default:
		break;
	}

	return ObjectProxyModel::data(index, role);
}
