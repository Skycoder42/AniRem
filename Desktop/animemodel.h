#ifndef ANIMEMODEL_H
#define ANIMEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <animeinfo.h>
#include <animestore.h>

class AnimeModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit AnimeModel(AnimeStore *store, QObject *parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	AnimePtr animeInfo(const QModelIndex &index);
	void uncheckAnime(const QModelIndex &index);

public slots:
	void addAnime(const AnimePtr &info);
	AnimePtr removeInfo(const QModelIndex &index);

private slots:
	void setAnimeList(const AnimeList &infoList);

private:
	AnimeList currentList;
	AnimeStore *store;
};

#endif // ANIMEMODEL_H
