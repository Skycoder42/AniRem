#ifndef PROXERENTRYMODEL_H
#define PROXERENTRYMODEL_H

#include <QAbstractTableModel>
#include <userclass.h>
#include <syncedsettings.h>

class ProxerEntryModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum Roles {
		NameRole = Qt::DisplayRole,
		TypeRole = Qt::UserRole + 1,
		DateRole,
		RatingRole,

		InvalidRole = -1
	};
	Q_ENUM(Roles)

	static const int PageSize = 50;

	explicit ProxerEntryModel(SyncedSettings *settings, QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = {}) const override;
	int columnCount(const QModelIndex &parent = {}) const override;
	QHash<int, QByteArray> roleNames() const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	bool canFetchMore(const QModelIndex &parent = {}) const override;
	void fetchMore(const QModelIndex &parent = {}) override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
	void loginNeeded();
	void apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType);

private:
	SyncedSettings *_settings;
	UserClass *_user;
	bool _isFetching;

	QList<ProxerListValue> _data;

	Roles indexRole(const QModelIndex &index, int role) const;
};

#endif // PROXERENTRYMODEL_H
