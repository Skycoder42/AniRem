#include "objectlistmodel.h"
#include <QMetaProperty>

ObjectListModel::ObjectListModel(const QMetaObject *objectType, bool objectOwner, QObject *parent) :
	QAbstractListModel(parent),
	_objectOwner(objectOwner),
	_metaObject(objectType),
	_roleNames(),
	_editable(true),
	_objects()
{
	auto roleIndex = Qt::UserRole + 1;
	for(auto i = 1; i < _metaObject->propertyCount(); i++)
		_roleNames.insert(roleIndex++, _metaObject->property(i).name());
	_roleNames.insert(Qt::DisplayRole, _metaObject->property(0).name());
}

QObjectList ObjectListModel::objects() const
{
	return _objects;
}

QObject *ObjectListModel::objectAt(int index) const
{
	return _objects[index];
}

QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QString::fromLatin1(_metaObject->className());
	else
		return {};
}

int ObjectListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return _objects.size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
	if (!testValid(index, role))
		return {};

	auto pIndex = _metaObject->indexOfProperty(_roleNames[role].constData());
	if(pIndex == -1)
		return {};
	else
		return _metaObject->property(pIndex).read(_objects[index.row()]);
}

bool ObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!_editable || !flags(index).testFlag(Qt::ItemIsEditable))
		return false;
	if (!testValid(index, role))
		return false;

	if (data(index, role) != value) {
		auto pIndex = _metaObject->indexOfProperty(_roleNames[role].constData());
		if(pIndex != -1) {
			auto ok = _metaObject->property(pIndex).write(_objects[index.row()], value);
			if(ok)
				emit dataChanged(index, index, QVector<int>() << role);
			return ok;
		}
	}

	return false;
}

Qt::ItemFlags ObjectListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsDropEnabled;

	return Qt::ItemIsEnabled |
			Qt::ItemIsSelectable |
			Qt::ItemIsEditable |
			Qt::ItemIsDragEnabled;
}

QHash<int, QByteArray> ObjectListModel::roleNames() const
{
	return _roleNames;
}

bool ObjectListModel::editable() const
{
	return _editable;
}

void ObjectListModel::addObject(QObject *object)
{
	beginInsertRows(QModelIndex(), _objects.size(), _objects.size());
	_objects.append(object);
	if(_objectOwner)
		object->setParent(this);
	endInsertRows();
}

void ObjectListModel::insertObject(const QModelIndex &index, QObject *object)
{
	insertObject(index.row(), object);
}

void ObjectListModel::insertObject(int index, QObject *object)
{
	beginInsertRows(QModelIndex(), index, index);
	_objects.insert(index, object);
	if(_objectOwner)
		object->setParent(this);
	endInsertRows();
}

void ObjectListModel::removeObject(const QModelIndex &index)
{
	removeObject(index.row());
}

void ObjectListModel::removeObject(int index)
{
	beginRemoveRows(QModelIndex(), index, index);
	auto obj = _objects.takeAt(index);
	if(_objectOwner && obj->parent() == this)
		obj->deleteLater();
	endRemoveRows();
}

void ObjectListModel::resetModel(QObjectList objects)
{
	beginResetModel();
	if(_objectOwner) {
		foreach(auto obj, _objects) {
			if(obj->parent() == this)
				obj->deleteLater();
		}
	}
	_objects = objects;
	endResetModel();
}

void ObjectListModel::setEditable(bool editable)
{
	if (_editable == editable)
		return;

	_editable = editable;
	emit editableChanged(editable);
}

bool ObjectListModel::testValid(const QModelIndex &index, int role) const
{
	return index.isValid() &&
			index.column() == 0 &&
			index.row() < _objects.size() &&
			(role < 0 || _roleNames.contains(role));
}
