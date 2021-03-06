#ifndef PROXERENTRYMODEL_H
#define PROXERENTRYMODEL_H

#include <QAbstractTableModel>
#include <userclass.h>
#include <syncedsettings.h>

class ProxerEntryModel : public QAbstractTableModel
{
	Q_OBJECT

	Q_PROPERTY(ListType listType READ listType WRITE setListType NOTIFY listTypeChanged)

public:
	enum ListType {
		ListAnimes = 0,
		ListMangas = 1
	};
	Q_ENUM(ListType)

	enum Roles {
		NameRole = Qt::DisplayRole,
		IdRole = Qt::UserRole + 1,
		TypeRole,
		DateRole,
		RatingRole,

		InvalidRole = -1
	};
	Q_ENUM(Roles)

	static const int PageSize;

	explicit ProxerEntryModel(SyncedSettings *settings, QObject *parent = nullptr);

	int getId(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = {}) const override;
	int columnCount(const QModelIndex &parent = {}) const override;
	QHash<int, QByteArray> roleNames() const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	bool canFetchMore(const QModelIndex &parent = {}) const override;
	void fetchMore(const QModelIndex &parent = {}) override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	ListType listType() const;

public slots:
	void setListType(ListType listType);

signals:
	void loginNeeded();
	void apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType);

	void listTypeChanged(ListType listType);

private:
	SyncedSettings *_settings;
	UserClass *_user;
	ListType _listType;
	bool _isFetching;
	bool _skipNext;

	QList<ProxerListValue> _data;

	Roles indexRole(const QModelIndex &index, int role) const;
	QString typeName(ListType listType) const;
};

#endif // PROXERENTRYMODEL_H
