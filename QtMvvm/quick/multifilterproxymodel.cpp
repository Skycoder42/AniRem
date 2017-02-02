#include "multifilterproxymodel.h"

MultiFilterProxyModel::MultiFilterProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent),
	_filterRoles()
{}

void MultiFilterProxyModel::addFilterRole(QByteArray roleName)
{
	auto role = roleNames().key(roleName, -1);
	if(role != -1)
		addFilterRole(role);
}

void MultiFilterProxyModel::addFilterRole(int role)
{
	_filterRoles.insert(role);
	invalidateFilter();
}

void MultiFilterProxyModel::clearFilterRoles()
{
	_filterRoles.clear();
	invalidateFilter();
}

void MultiFilterProxyModel::setFilter(const QRegularExpression &regex)
{
	_filterRegex = regex;
	invalidateFilter();
}

bool MultiFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	if(!_filterRegex.isValid() || _filterRoles.isEmpty())
		return true;

	foreach(auto role, _filterRoles) {
		auto rData = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), role);
		auto strList = rData.toStringList();
		foreach(auto str, strList) {
			if(_filterRegex.match(str).hasMatch())
				return true;
		}
		auto str = rData.toString();
		if(!str.isNull() && _filterRegex.match(str).hasMatch())
			return true;
	}

	return false;
}
