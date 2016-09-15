#include "animeseasonmodel.h"
#include <QLocale>
#include <QFont>

AnimeSeasonModel::AnimeSeasonModel(QObject *parent) :
	QAbstractTableModel(parent),
	seasonList(),
	store(new AnimeStore(this))
{
	connect(this->store, &AnimeStore::loadingCompleted,
			this, &AnimeSeasonModel::storeLoaded,
			Qt::QueuedConnection);
	connect(this->store, &AnimeStore::storeError,
			this, &AnimeSeasonModel::modelError,
			Qt::QueuedConnection);

	QMetaObject::invokeMethod(this->store, "loadAnimes", Qt::QueuedConnection);
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
			return this->seasonList[index.row()].lastKnownSeasons < 0 ?
					tr("Not loaded") :
					QLocale().toString(this->seasonList[index.row()].lastKnownSeasons);
		case 2:
			return tr("proxer.me/info/%1/relation").arg(this->seasonList[index.row()].id);
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(index.column() == 1 &&
		   this->seasonList[index.row()].lastKnownSeasons < 0) {
			QFont font;
			font.setItalic(true);
			return font;
		} else
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

QList<AnimeInfo> AnimeSeasonModel::animeList() const
{
	return this->seasonList;
}

void AnimeSeasonModel::addAnime(const AnimeInfo &info)
{
	this->beginInsertRows(QModelIndex(), this->seasonList.size(), this->seasonList.size());
	this->seasonList.append(info);
	this->store->saveAnime(info);
	this->endInsertRows();
}

void AnimeSeasonModel::removeInfo(const QModelIndex &index)
{
	this->beginRemoveRows(index.parent(), index.row(), index.row());
	auto info = this->seasonList.takeAt(index.row());
	this->store->forgetAnime(info.id);
	this->endRemoveRows();
}

void AnimeSeasonModel::storeLoaded(QList<AnimeInfo> seasonList)
{
	this->beginResetModel();
	this->seasonList = seasonList;
	this->endResetModel();
}
