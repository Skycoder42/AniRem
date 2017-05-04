#ifndef DATASYNCOBJECTMODEL_H
#define DATASYNCOBJECTMODEL_H

#include <QObject>
#include <setup.h>
#include <cachingdatastore.h>
#include <qobjectlistmodel.h>

template <typename T, typename K = QString>
class DatasyncObjectModel : public QGenericListModel<T>
{
public:
	explicit DatasyncObjectModel(QObject *parent = nullptr);
	explicit DatasyncObjectModel(const QString &setupName, QObject *parent = nullptr);
	explicit DatasyncObjectModel(QtDataSync::CachingDataStore<T*, K> *store, QObject *parent = nullptr);

	QtDataSync::CachingDataStore<T*, K> *store() const;

private:
	QtDataSync::CachingDataStore<T*, K> *_store;
	QSet<QString> _activeKeys;

	//"removed" methods
	void addObject(T *object);
	void insertObject(const QModelIndex &index, T *object);
	void insertObject(int index, T *object);
	void removeObject(const QModelIndex &index);
	void removeObject(int index);
	T *takeObject(const QModelIndex &index);
	T *takeObject(int index);
	void resetModel(QList<T*> objects);

	QString getKey(QObject *obj) const;

	void onLoaded();
	void onReset();
	void onChanged(const QString &key, const QVariant &value);
};

template<typename T, typename K>
DatasyncObjectModel<T, K>::DatasyncObjectModel(QObject *parent) :
	DatasyncObjectModel(QtDataSync::Setup::DefaultSetup, parent)
{}

template<typename T, typename K>
DatasyncObjectModel<T, K>::DatasyncObjectModel(const QString &setupName, QObject *parent) :
	QGenericListModel<T>(false, parent),
	_store(new QtDataSync::CachingDataStore<T*, K>(setupName, this)),
	_activeKeys()
{
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::storeLoaded,
					 this, &DatasyncObjectModel<T, K>::onLoaded);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataResetted,
					 this, &DatasyncObjectModel<T, K>::onReset);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataChanged,
					 this, &DatasyncObjectModel<T, K>::onChanged);
}

template<typename T, typename K>
DatasyncObjectModel<T, K>::DatasyncObjectModel(QtDataSync::CachingDataStore<T*, K> *store, QObject *parent) :
	QGenericListModel<T>(false, parent),
	_store(store),
	_activeKeys()
{
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::storeLoaded,
					 this, &DatasyncObjectModel<T, K>::onLoaded);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataResetted,
					 this, &DatasyncObjectModel<T, K>::onReset);
	QObject::connect(_store, &QtDataSync::CachingDataStoreBase::dataChanged,
					 this, &DatasyncObjectModel<T, K>::onChanged);
}

template<typename T, typename K>
QtDataSync::CachingDataStore<T*, K> *DatasyncObjectModel<T, K>::store() const
{
	return _store;
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::addObject(T *object)
{
	Q_UNUSED(object);
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::insertObject(const QModelIndex &index, T *object)
{
	Q_UNUSED(index);
	Q_UNUSED(object);
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::insertObject(int index, T *object)
{
	Q_UNUSED(index);
	Q_UNUSED(object);
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::removeObject(const QModelIndex &index)
{
	Q_UNUSED(index);
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::removeObject(int index)
{
	Q_UNUSED(index);
}

template<typename T, typename K>
T *DatasyncObjectModel<T, K>::takeObject(const QModelIndex &index)
{
	Q_UNUSED(index);
	return nullptr;
}

template<typename T, typename K>
T *DatasyncObjectModel<T, K>::takeObject(int index)
{
	Q_UNUSED(index);
	return nullptr;
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::resetModel(QList<T *> objects)
{
	Q_UNUSED(objects);
}

template<typename T, typename K>
QString DatasyncObjectModel<T, K>::getKey(QObject *obj) const
{
	auto user = obj->metaObject()->userProperty();
	return user.read(obj).toString();
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::onLoaded()
{
	_activeKeys.clear();
	foreach (auto key, _store->keys())
		_activeKeys.insert(QVariant::fromValue(key).toString());
	QGenericListModel<T>::resetModel(_store->loadAll());
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::onReset()
{
	_activeKeys.clear();
	QGenericListModel<T>::resetModel({});
}

template<typename T, typename K>
void DatasyncObjectModel<T, K>::onChanged(const QString &key, const QVariant &value)
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
