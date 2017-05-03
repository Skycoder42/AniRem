#ifndef DATASYNCOBJECTMODEL_H
#define DATASYNCOBJECTMODEL_H

#include <QObject>
#include <setup.h>
#include <cachingdatastore.h>
#include <qobjectlistmodel.h>

template <typename T>
class DatasyncObjectModel : public QGenericListModel<T>
{
public:
	explicit DatasyncObjectModel(QObject *parent = nullptr);
	explicit DatasyncObjectModel(const QString &setupName, QObject *parent = nullptr);

	void addObject(T *object);
	void insertObject(const QModelIndex &index, T *object);
	void insertObject(int index, T *object);
	void removeObject(const QModelIndex &index);
	void removeObject(int index);

private:
	QtDataSync::CachingDataStore<T*> *_store;
	QSet<QString> _activeKeys;

	//"removed" methods
	T *takeObject(const QModelIndex &index);
	T *takeObject(int index);
	void resetModel(QList<T*> objects);

	QString getKey(QObject *obj) const;

	void onLoaded();
	void onReset();
	void onChanged(const QString &key, const QVariant &value);
};

template<typename T>
DatasyncObjectModel<T>::DatasyncObjectModel(QObject *parent) :
	DatasyncObjectModel(QtDataSync::Setup::DefaultSetup, parent)
{}

template<typename T>
DatasyncObjectModel<T>::DatasyncObjectModel(const QString &setupName, QObject *parent) :
	QGenericListModel<T>(false, parent),
	_store(new QtDataSync::CachingDataStore<T*>(setupName, parent)),
	_activeKeys()
{
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::storeLoaded,
			this, &DatasyncObjectModel<T>::onLoaded);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataResetted,
			this, &DatasyncObjectModel<T>::onReset);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataChanged,
			this, &DatasyncObjectModel<T>::onChanged);
}

template<typename T>
void DatasyncObjectModel<T>::addObject(T *object)
{
	_activeKeys.insert(getKey(object));
	_store->save(object);
	QObjectListModel::addObject(object);
}

template<typename T>
void DatasyncObjectModel<T>::insertObject(const QModelIndex &index, T *object)
{
	_activeKeys.insert(getKey(object));
	_store->save(object);
	QObjectListModel::insertObject(index, object);
}

template<typename T>
void DatasyncObjectModel<T>::insertObject(int index, T *object)
{
	_activeKeys.insert(getKey(object));
	_store->save(object);
	QObjectListModel::insertObject(index, object);
}

template<typename T>
void DatasyncObjectModel<T>::removeObject(const QModelIndex &index)
{
	auto obj = QObjectListModel::takeObject(index);
	auto key = getKey(obj);
	_activeKeys.remove(key);
	_store->remove(key);
}

template<typename T>
void DatasyncObjectModel<T>::removeObject(int index)
{
	auto obj = QObjectListModel::takeObject(index);
	auto key = getKey(obj);
	_activeKeys.remove(key);
	_store->remove(key);
}

template<typename T>
T *DatasyncObjectModel<T>::takeObject(const QModelIndex &index)
{
	removeObject(index);
	return nullptr;
}

template<typename T>
T *DatasyncObjectModel<T>::takeObject(int index)
{
	removeObject(index);
	return nullptr;
}

template<typename T>
void DatasyncObjectModel<T>::resetModel(QList<T *> objects)
{
	Q_UNUSED(objects);
}

template<typename T>
QString DatasyncObjectModel<T>::getKey(QObject *obj) const
{
	auto user = obj->metaObject()->userProperty();
	return user.read(obj).toString();
}

template<typename T>
void DatasyncObjectModel<T>::onLoaded()
{
	_activeKeys = QSet<QString>::fromList(_store->keys());
	QGenericListModel<T>::resetModel(_store->loadAll());
}

template<typename T>
void DatasyncObjectModel<T>::onReset()
{
	_activeKeys = QSet<QString>::fromList(_store->keys());
	QGenericListModel<T>::resetModel(_store->loadAll());
}

template<typename T>
void DatasyncObjectModel<T>::onChanged(const QString &key, const QVariant &value)
{
	if(value.isValid()) {// added/changed
		if(!_activeKeys.contains(key)) {// added
			auto obj = value.value<T*>();
			if(obj) {
				_activeKeys.insert(key);
				QObjectListModel::addObject(obj);
			}
		} //ignore changed, works automatically
	} else {// removed
		if(_activeKeys.contains(key)) {
			auto objs = QGenericListModel<T>::objects();
			foreach (auto obj, objs) {
				if(key == getKey(obj)) {
					QObjectListModel::removeObject(QObjectListModel::index(obj));
					_activeKeys.remove(key);
					break;
				}
			}
		}
	}
}

#endif // DATASYNCOBJECTMODEL_H
