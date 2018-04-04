#include "animemodel.h"

#include <QColor>
#include <QFont>
#include <QLocale>

AnimeModel::AnimeModel(QtDataSync::DataStoreModel *srcModel, QObject *parent) :
	QObjectProxyModel({tr("Id"), tr("Name"), tr("Season Count"), tr("Last updated"), tr("Season Overview")}, parent)
{
	setSourceModel(srcModel);

	addMapping(0, Qt::DisplayRole, "id");
	addMapping(0, Qt::ToolTipRole, "id");

	addMapping(1, Qt::DisplayRole, "title");
	addMapping(1, Qt::ToolTipRole, "title");

	addMapping(2, Qt::DisplayRole, "totalSeasonCount");
	addMapping(2, Qt::ToolTipRole, "totalSeasonCount");

	addMapping(3, Qt::DisplayRole, "lastUpdateCheck");
	addMapping(3, Qt::ToolTipRole, "lastUpdateCheck");

	addMapping(4, Qt::DisplayRole, "relationsUrl");
	addMapping(4, Qt::ToolTipRole, "relationsUrl");
}

QVariant AnimeModel::data(const QModelIndex &index, int role) const
{
	auto src = mapToSource(index);
	if(!src.isValid())
		return {};

	auto info = static_cast<QtDataSync::DataStoreModel*>(sourceModel())->object<AnimeInfo>(src);
	switch (role) {
	case Qt::DisplayRole:
	case Qt::ToolTipRole:
		switch(index.column()) {
		case 2:
			return QLocale().toString(info.totalSeasonCount());
		case 3:
			return info.lastUpdateCheck().toString(Qt::DefaultLocaleShortDate);
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(info.hasNewSeasons()) {
			QFont font;
			font.setBold(true);
			return font;
		} else
			break;
	case Qt::ForegroundRole:
		if(info.hasNewSeasons())
			return QColor(0x8A, 0x0E, 0x0E);
		else
			break;
	default:
		break;
	}

	return QObjectProxyModel::data(index, role);
}
