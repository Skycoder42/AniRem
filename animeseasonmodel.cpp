#include "animeseasonmodel.h"

AnimeSeasonModel::AnimeSeasonModel(QObject *parent) :
	QAbstractTableModel(parent),
	seasonList()
{
}

QVariant AnimeSeasonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation != Qt::Horizontal || role != Qt::DisplayRole)
		return QVariant();
	else switch (section) {
	case 0:
		return tr("Name");
	case 1:
		return tr("Current Seasons");
	case 2:
		return tr("Season Overview");
	default:
		return QVariant();
	}
}

int AnimeSeasonModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return this->seasonList.size();
}

int AnimeSeasonModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return 3;
}

QVariant AnimeSeasonModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() ||
		index.row() < 0 ||
		index.row() >= this->seasonList.size() ||
		index.column() < 0 ||
		index.column() >= 3)
		return QVariant();

	switch (role) {
	case Qt::DisplayRole:
		switch(index.column()) {
		case 0:
			return this->seasonList[index.row()].title;
		case 1:
			return this->seasonList[index.row()].lastKnownSeasons;
		case 2:
			return tr("proxer.me/info/%1/relation").arg(this->seasonList[index.row()].id);
		default:
			break;
		}
		break;
	default:
		break;
	}

	return QVariant();
}

void AnimeSeasonModel::addAnime(const AnimeInfo &info)
{
	this->beginInsertRows(QModelIndex(), this->seasonList.size(), this->seasonList.size());
	this->seasonList.append(info);
	this->endInsertRows();
}

void AnimeSeasonModel::removeInfo(int index)
{
	this->beginRemoveRows(QModelIndex(), index, index);
	this->seasonList.removeAt(index);
	this->endRemoveRows();
}
