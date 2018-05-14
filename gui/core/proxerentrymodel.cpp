#include "proxerentrymodel.h"
#include <animeinfo.h>
#include <apihelper.h>
#include <proxerapi.h>

const int ProxerEntryModel::PageSize = 50;

ProxerEntryModel::ProxerEntryModel(SyncedSettings *settings, QObject *parent) :
	QAbstractTableModel(parent),
	_settings(settings),
	//WORKAROUND _user(ProxerApi::factory().user().instance(this)),
	_user((new ProxerApi(this))->user()),
	_listType(ListAnimes),
	_isFetching(false),
	_skipNext(false),
	_data()
{
	_user->setErrorTranslator(&ApiHelper::transformError);
	connect(_user, &UserClass::apiError,
			this, &ProxerEntryModel::apiError);
	connect(this, &ProxerEntryModel::apiError,
			this, [this](){
		_isFetching = false;
	});
}

int ProxerEntryModel::getId(const QModelIndex &index) const
{
	if(index.isValid())
		return _data.value(index.row()).id();
	else
		return -1;
}

int ProxerEntryModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return _data.size();
}

int ProxerEntryModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return 4;
}

QHash<int, QByteArray> ProxerEntryModel::roleNames() const
{
	return {
		{NameRole, "name"},
		{IdRole, "id"},
		{TypeRole, "type"},
		{DateRole, "date"},
		{RatingRole, "rating"}
	};
}

QVariant ProxerEntryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation != Qt::Horizontal ||
	   role != Qt::DisplayRole)
		return {};

	switch(section) {
	case 0:
		return tr("Name");
	case 1:
		return tr("Type");
	case 2:
		return tr("Last modified");
	case 3:
		return tr("Rating");
	default:
		return {};
	}
}

bool ProxerEntryModel::canFetchMore(const QModelIndex &parent) const
{
	if(parent.isValid())
		return false;
	else
		return (_data.size() % PageSize) == 0;
}

void ProxerEntryModel::fetchMore(const QModelIndex &parent)
{
	qDebug() << "Trying to fetch" << (!_isFetching && !_skipNext);
	if(!canFetchMore(parent) || _isFetching)
		return;

	if(_skipNext) {
		_skipNext = false;
		return;
	}

	_isFetching = true;
	auto res = _user->listEntries(typeName(_listType),
								  _settings->content.hentai,
								  _data.size() / PageSize,
								  PageSize);
	res->onSucceeded([this](int code, ProxerList entryList){
		_isFetching = false;
		if(ApiHelper::testValid(code, entryList)) {
			beginInsertRows({}, _data.size(), _data.size() + entryList.data().size() - 1);
			_data.append(entryList.data());
			endInsertRows();
			_skipNext = true;
		} else {
			if(entryList.code() == 3003) // user does not exist -> means a login is needed
				emit loginNeeded();
			else
				emit apiError(entryList.message(), entryList.code(), QtRestClient::RestReply::FailureError);
		}
	});
}

QVariant ProxerEntryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch(indexRole(index, role)) {
	case NameRole:
		return _data.value(index.row()).name();
	case IdRole:
		return _data.value(index.row()).id();
	case TypeRole:
		return std::get<0>(AnimeInfo::apiMediumToType(_data.value(index.row()).medium(), _settings));
	case DateRole:
		return QDateTime::fromTime_t(_data.value(index.row()).timestamp());
	case RatingRole:
		return _data.value(index.row()).rating();
	default:
		return {};
	}
}

ProxerEntryModel::ListType ProxerEntryModel::listType() const
{
	return _listType;
}

void ProxerEntryModel::setListType(ProxerEntryModel::ListType listType)
{
	if (_listType == listType)
		return;

	_listType = listType;
	emit listTypeChanged(_listType);

	// reset the model as all data will change now
	beginResetModel();
	_data.clear();
	_isFetching = false;
	_skipNext = false;
	endResetModel();
}

ProxerEntryModel::Roles ProxerEntryModel::indexRole(const QModelIndex &index, int role) const
{
	if(index.column() == 0)
		return static_cast<Roles>(role);
	else if(role == Qt::DisplayRole) {
		switch(index.column()) {
		case 0:
			return NameRole;
		case 1:
			return TypeRole;
		case 2:
			return DateRole;
		case 3:
			return RatingRole;
		default:
			break;
		}
	}

	return InvalidRole;
}

QString ProxerEntryModel::typeName(ProxerEntryModel::ListType listType) const
{
	switch(listType) {
	case ListAnimes:
		return QStringLiteral("anime");
	case ListMangas:
		return QStringLiteral("manga");
	default:
		Q_UNREACHABLE();
		return {};
	}
}
