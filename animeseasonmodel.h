#ifndef ANIMESEASONMODEL_H
#define ANIMESEASONMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "animeinfo.h"
#include "animestore.h"

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

	AnimeInfo animeInfo(const QModelIndex &index);
	QList<AnimeInfo> animeList() const;
	void uncheckAnime(const QModelIndex &index);

public slots:
	void addAnime(const AnimeInfo &info);
	void removeInfo(const QModelIndex &index);

signals:
	void modelError(QString error);

private slots:
	void storeLoaded(QList<AnimeInfo> seasonList);

private:
	QList<AnimeInfo> seasonList;

	AnimeStore *store;
};

#endif // ANIMESEASONMODEL_H
