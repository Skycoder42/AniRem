#include "animemodel.h"
#include "app.h"

#include <QFont>
#include <QLocale>

AnimeModel::AnimeModel(AnimeStore *store, QObject *parent) :
	QAbstractTableModel(parent),
	currentList(),
	store(store)
{
	connect(store, &AnimeStore::animeInfoListChanged,
			this, &AnimeModel::setAnimeList);
}

QVariant AnimeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation != Qt::Horizontal || role != Qt::DisplayRole)
		return QVariant();
	else switch (section) {
	case 0:
		return tr("Name");
	case 1:
		return tr("Season Count");
	case 2:
		return tr("Season Overview");
	default:
		return QVariant();
	}
}

int AnimeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return currentList.size();
}

int AnimeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return 3;
}

QVariant AnimeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() ||
		index.row() < 0 ||
		index.row() >= currentList.size() ||
		index.column() < 0 ||
		index.column() >= 3)
		return QVariant();

	switch (role) {
	case Qt::DisplayRole:
		switch(index.column()) {
		case 0:
			return currentList[index.row()]->title();
		case 1:
			return currentList[index.row()]->lastKnownSeasons() < 0 ?
					tr("Not loaded") :
					QLocale().toString(currentList[index.row()]->lastKnownSeasons());
		case 2:
			return currentList[index.row()]->relationsUrl();
		default:
			break;
		}
		break;
	case Qt::FontRole:
		if(currentList[index.row()]->hasNewSeasons()) {
			QFont font;
			font.setBold(true);
			return font;
		} else if(index.column() == 1 &&
				  currentList[index.row()]->lastKnownSeasons() < 0) {
			QFont font;
			font.setItalic(true);
			return font;
		} else
			break;
	case Qt::ForegroundRole:
		if(currentList[index.row()]->hasNewSeasons())
			return QColor(Qt::red);
		else
			break;
	default:
		break;
	}

	return QVariant();
}

AnimePtr AnimeModel::animeInfo(const QModelIndex &index)
{
	if (!index.isValid() ||
		 index.row() < 0 ||
		 index.row() >= currentList.size())
		 return {};
	 else
		 return currentList[index.row()];
}

void AnimeModel::uncheckAnime(const QModelIndex &index)
{
	if (index.isValid() &&
		index.row() >= 0 &&
		index.row() < currentList.size()) {
		currentList[index.row()]->setHasNewSeasons(false);
		emit dataChanged(index.sibling(index.row(), 0),
						 index.sibling(index.row(), 2));
		store->saveAnime(currentList[index.row()]);
	}
}

void AnimeModel::addAnime(const AnimePtr &info)
{
	if(store->containsAnime(info->id()))
		qApp->showMessage(QMessageBox::Warning, tr("Anime duplicated"), tr("Anime \"%1\" is already in the list!").arg(info->title()));
	else {
		beginInsertRows(QModelIndex(), currentList.size(), currentList.size());
		currentList.append(info);
		endInsertRows();
		store->saveAnime(info);
	}
}

AnimePtr AnimeModel::removeInfo(const QModelIndex &index)
{
	beginRemoveRows(index.parent(), index.row(), index.row());
	auto info = currentList.takeAt(index.row());
	endRemoveRows();
	store->forgetAnime(info->id());
	return info;
}

void AnimeModel::setAnimeList(const AnimeList &infoList)
{
	beginResetModel();
	currentList = infoList;
	endResetModel();
}
