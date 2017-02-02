#ifndef ANIMEMODEL_H
#define ANIMEMODEL_H

#include <QObject>
#include <objectproxymodel.h>
#include <animeinfo.h>

class AnimeModel : public ObjectProxyModel
{
	Q_OBJECT

public:
	explicit AnimeModel(GenericListModel<AnimeInfo> *srcModel, QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // ANIMEMODEL_H
