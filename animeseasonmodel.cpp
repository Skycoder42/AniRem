#include "animeseasonmodel.h"
#include <QLocale>
#include <QFont>

AnimeSeasonModel::AnimeSeasonModel(AnimeStore *store, QObject *parent) :
	QAbstractTableModel(parent),
	seasonList(),
	store(store)
{
	connect(this->store, &AnimeStore::animeInfoListChanged,
			this, &AnimeSeasonModel::setAnimeList);
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
			return this->seasonList[index.row()].title();
		case 1:
			return this->seasonList[index.row()].lastKnownSeasons() < 0 ?
					tr("Not loaded") :
					QLocale().toString(this->seasonList[index.row()].lastKnownSeasons());
		case 2:
			return this->seasonList[index.row()].relationsUrl();
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(this->seasonList[index.row()].hasNewSeasons()) {
			QFont font;
			font.setBold(true);
			return font;
		} else if(index.column() == 1 &&
				  this->seasonList[index.row()].lastKnownSeasons() < 0) {
			QFont font;
			font.setItalic(true);
			return font;
		} else
			break;
	case Qt::ForegroundRole:
		if(this->seasonList[index.row()].hasNewSeasons())
			return QColor(Qt::red);
		else
			break;
	default:
		break;
	}

	return QVariant();
}

AnimeInfo AnimeSeasonModel::animeInfo(const QModelIndex &index)
{
	if (!index.isValid() ||
		index.row() < 0 ||
		index.row() >= this->seasonList.size())
		return AnimeInfo();
	else
		return this->seasonList[index.row()];
}

void AnimeSeasonModel::uncheckAnime(const QModelIndex &index)
{
	if (index.isValid() &&
		index.row() >= 0 &&
		index.row() < this->seasonList.size()) {
		this->seasonList[index.row()].setHasNewSeasons(false);
		emit dataChanged(index.sibling(index.row(), 0),
						 index.sibling(index.row(), 2));
		this->store->saveAnime(this->seasonList[index.row()]);
	}
}

void AnimeSeasonModel::addAnime(const AnimeInfo &info)
{
	this->beginInsertRows(QModelIndex(), this->seasonList.size(), this->seasonList.size());
	this->seasonList.append(info);
	this->endInsertRows();
	this->store->saveAnime(info);
}

AnimeInfo AnimeSeasonModel::removeInfo(const QModelIndex &index)
{
	this->beginRemoveRows(index.parent(), index.row(), index.row());
	auto info = this->seasonList.takeAt(index.row());
	this->endRemoveRows();
	this->store->forgetAnime(info.id());
	return info;
}

void AnimeSeasonModel::setAnimeList(const QList<AnimeInfo> &infoList)
{
	this->beginResetModel();
	this->seasonList = infoList;
	this->endResetModel();
}
