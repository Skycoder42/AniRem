#include "entryviewmodel.h"
#include <QtMvvmCore/Messages>
#include <apihelper.h>
#include <proxerapi.h>
#include "loginviewmodel.h"

EntryViewModel::EntryViewModel(QObject *parent) :
	ViewModel(parent),
	_settings(nullptr),
	_model(nullptr),
	_showingLogin(false)
{}

ProxerEntryModel *EntryViewModel::model() const
{
	return _model;
}

void EntryViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params)

	_model = new ProxerEntryModel(_settings, this);
	connect(_model, &ProxerEntryModel::loginNeeded,
			this, &EntryViewModel::loginNeeded);
	connect(_model, &ProxerEntryModel::apiError,
			this, &EntryViewModel::apiError);
	emit modelChanged(_model);
}

void EntryViewModel::onResult(quint32 requestCode, const QVariant &result)
{
	if(requestCode == LoginRequestCode) {
		_showingLogin = false;
		if(result.toBool())
			_model->fetchMore();
	}
}

void EntryViewModel::loginNeeded()
{
	if(!_showingLogin) {
		_showingLogin = true;
		showForResult<LoginViewModel>(LoginRequestCode);
	}
}

void EntryViewModel::apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType)
{
	QtMvvm::critical(tr("Failed to get entries"),
					 ApiHelper::formatError(errorString, errorCode, errorType));
}
