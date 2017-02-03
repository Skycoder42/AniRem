#include "animemodel.h"

#include <QFont>
#include <QLocale>

AnimeModel::AnimeModel(GenericListModel<AnimeInfo> *srcModel, QObject *parent) :
	ObjectProxyModel({tr("Id"), tr("Name"), tr("Season Count"), tr("Season Overview")}, parent)
{
	setSourceModel(srcModel);
	addMapping(0, Qt::DisplayRole, "id");
	addMapping(1, Qt::DisplayRole, "title");
	addMapping(2, Qt::DisplayRole, "totalSeasonCount");
	addMapping(3, Qt::DisplayRole, "relationsUrl");
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
		case 2:
			return info->seasonState().isEmpty() ?
					tr("Not loaded") :
					QLocale().toString(info->totalSeasonCount());
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(info->hasNewSeasons()) {
			QFont font;
			font.setBold(true);
			return font;
		} else if(index.column() == 2 &&
				  info->seasonState().isEmpty()) {
			QFont font;
			font.setItalic(true);
			return font;
		} else
			break;
	case Qt::ForegroundRole:
		if(info->hasNewSeasons())
			return QColor(0x8A, 0x0E, 0x0E);
		else
			break;
	default:
		break;
	}

	return ObjectProxyModel::data(index, role);
}
