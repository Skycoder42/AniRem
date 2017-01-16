#ifndef OBJECTPROXYMODEL_H
#define OBJECTPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QObject>
#include <objectlistmodel.h>

class ObjectProxyModel : public QAbstractProxyModel
{
	Q_OBJECT

public:
	explicit ObjectProxyModel(QStringList headers, QObject *parent = nullptr);

	void addMapping(int column, int role, int sourceRole);

public:
	QModelIndex index(int row, int column, const QModelIndex &parent = {}) const;
	QModelIndex parent(const QModelIndex &) const;
	int rowCount(const QModelIndex &parent = {}) const;
	int columnCount(const QModelIndex &parent = {}) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QHash<int, QByteArray> roleNames() const;

	void setSourceModel(ObjectListModel *sourceModel);
	QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

private:
	QStringList _headers;
	QHash<QPair<int, int>, int> _roleMapping;

	mutable bool _roleNamesChanged;
	mutable QHash<int, QByteArray> _roleNames;

	void setSourceModel(QAbstractItemModel *sourceModel);
};

#endif // OBJECTPROXYMODEL_H
