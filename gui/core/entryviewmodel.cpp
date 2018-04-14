#include "entryviewmodel.h"
#include <QtMvvmCore/Messages>
#include <apihelper.h>
#include <proxerapi.h>
#include "loginviewmodel.h"
#include "addanimeviewmodel.h"

EntryViewModel::EntryViewModel(QObject *parent) :
	ViewModel(parent),
	_settings(nullptr),
	_model(nullptr),
	_showingChild(false)
{}

ProxerEntryModel *EntryViewModel::model() const
{
	return _model;
}

int EntryViewModel::typeIndex() const
{
	return _model ? _model->listType() : ProxerEntryModel::ListAnimes;
}

QStringList EntryViewModel::listTypes() const
{
	return {
		tr("Animes"),
		tr("Mangas")
	};
}

void EntryViewModel::addAnime(int id)
{
	if(id == -1)
		return;

	if(!_showingChild) {
		_showingChild = true;
		showForResult<AddAnimeViewModel>(AddRequestCode, AddAnimeViewModel::params(id));
	}
}

void EntryViewModel::setTypeIndex(int typeIndex)
{
	if(_model)
		_model->setListType(static_cast<ProxerEntryModel::ListType>(typeIndex));
}

void EntryViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params)

	_model = new ProxerEntryModel(_settings, this);
	connect(_model, &ProxerEntryModel::listTypeChanged,
			this, &EntryViewModel::typeIndexChanged);
	connect(_model, &ProxerEntryModel::loginNeeded,
			this, &EntryViewModel::loginNeeded);
	connect(_model, &ProxerEntryModel::apiError,
			this, &EntryViewModel::apiError);
	emit modelChanged(_model);
}

void EntryViewModel::onResult(quint32 requestCode, const QVariant &result)
{
	switch(requestCode) {
	case LoginRequestCode:
		_showingChild = false;
		if(result.toBool())
			_model->fetchMore();
		break;
	case AddRequestCode:
		_showingChild = false;
		break;
	default:
		break;
	}
}

void EntryViewModel::loginNeeded()
{
	if(!_showingChild) {
		_showingChild = true;
		showForResult<LoginViewModel>(LoginRequestCode);
	}
}

void EntryViewModel::apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType)
{
	QtMvvm::critical(tr("Failed to get entries"),
					 ApiHelper::formatError(errorString, errorCode, errorType));
}
