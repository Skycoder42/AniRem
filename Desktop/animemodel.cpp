#include "animemodel.h"

#include <QFont>
#include <QLocale>

AnimeModel::AnimeModel(QGenericListModel<AnimeInfo> *srcModel, QObject *parent) :
	QObjectProxyModel({tr("Id"), tr("Name"), tr("Season Count"), tr("Season Overview")}, parent)
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

	auto info = static_cast<QGenericListModel<AnimeInfo>*>(sourceModel())->object(src);
	switch (role) {
	case Qt::DisplayRole:
		switch(index.column()) {
		case 2:
			return QLocale().toString(info->totalSeasonCount());
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(info->hasNewSeasons()) {
			QFont font;
			font.setBold(true);
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

	return QObjectProxyModel::data(index, role);
}
