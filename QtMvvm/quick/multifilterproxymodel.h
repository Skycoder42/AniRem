#ifndef MULTIFILTERPROXYMODEL_H
#define MULTIFILTERPROXYMODEL_H

#include <QObject>
#include <QRegularExpression>
#include <QSet>
#include <QSortFilterProxyModel>

class MultiFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit MultiFilterProxyModel(QObject *parent = nullptr);

	void addFilterRole(QByteArray roleName);
	void addFilterRole(int role);
	void clearFilterRoles();

	void setFilter(const QRegularExpression &regex);

protected:
	bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
	QSet<int> _filterRoles;
	QRegularExpression _filterRegex;
};

#endif // MULTIFILTERPROXYMODEL_H
