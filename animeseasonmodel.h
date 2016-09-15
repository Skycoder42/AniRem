#ifndef ANIMESEASONMODEL_H
#define ANIMESEASONMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "animeinfo.h"

class AnimeSeasonModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit AnimeSeasonModel(QObject *parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
	void addAnime(const AnimeInfo &info);
	void removeInfo(int index);

private:
	QList<AnimeInfo> seasonList;
};

#endif // ANIMESEASONMODEL_H
