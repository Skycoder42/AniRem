#include "addanimeviewmodel.h"
#include <QtMvvmCore/Messages>
#include <QtMvvmCore/CoreApp>
#include <proxerapi.h>
#include <infoclass.h>
#include <api/apihelper.h>

const QString AddAnimeViewModel::ParamId = QStringLiteral("id");

QVariantHash AddAnimeViewModel::params(int id)
{
	return {
		{ParamId, id}
	};
}

AddAnimeViewModel::AddAnimeViewModel(QObject *parent) :
	ViewModel(parent),
	_store(new AniremStore(this)),
	_infoClass(ProxerApi::factory().info().instance(this)),
	_loader(nullptr),
	_id(-1),
	_title(),
	_loading(false),
	_acceptable(false)
{
	connect(_infoClass, &InfoClass::apiError,
			this, &AddAnimeViewModel::error);
}

int AddAnimeViewModel::id() const
{
	return _id;
}

QString AddAnimeViewModel::title() const
{
	return _title;
}

bool AddAnimeViewModel::isLoading() const
{
	return _loading;
}

bool AddAnimeViewModel::isAcceptable() const
{
	return _acceptable;
}

ImageLoader *AddAnimeViewModel::imageLoader() const
{
	return _loader;
}

bool AddAnimeViewModel::accept(bool allowInvalid)
{
	if((allowInvalid || _acceptable) && _id != -1) {
		if(_title.isEmpty())
			setTitle(tr("Anime No. %1").arg(_id));
		try {
			_store->save({_id, _title});
			return true;
		} catch(QException &e) {
			qCritical() << "Failed to save entry with id" << _id
						<< "and error:" << e.what();
			return false;
		}
	} else
		return false;
}

void AddAnimeViewModel::setId(int id)
{
	if (_id == id)
		return;

	_id = id;
	emit idChanged(id);
	setTitle(QString());
	setLoading(true);

	auto rep = _infoClass->getEntry(_id);
	rep->onSucceeded([this](int code, ProxerEntry entry){
		if(ApiHelper::testValid(code, entry)) {
			if(entry.data().id() == _id) {
				setTitle(entry.data().name());
				setLoading(false);
				setAcceptable(true);
			}
		} else
		   error(entry.message(), code, QtRestClient::RestReply::FailureError);
	});
}

void AddAnimeViewModel::setTitle(QString title)
{
	if (_title == title)
		return;

	_title = title;
	emit titleChanged(title);
}

void AddAnimeViewModel::onInit(const QVariantHash &params)
{
	if(params.contains(ParamId))
		setId(params.value(ParamId).toInt());
}

void AddAnimeViewModel::error(const QString &errorString, int errorCode, int errorType)
{
	qCritical().noquote() << "Network error of type" << static_cast<QtRestClient::RestReply::ErrorType>(errorType)
						  << "with code" << errorCode << "and error message:" << errorString;
	setLoading(false);

	QtMvvm::MessageConfig config {
		QtMvvm::MessageConfig::TypeMessageBox,
		QtMvvm::MessageConfig::SubTypeCritical,
	};
	config.setTitle(tr("Network Error"))
			.setText(tr("Unable to download data about the Anime from the server!"))
			.setButtons(QtMvvm::MessageConfig::Retry | QtMvvm::MessageConfig::Cancel)
			.setButtonText(QtMvvm::MessageConfig::Save, tr("Save anyways"));

	auto res = coreApp->showDialog(config);
	connect(res, &QtMvvm::MessageResult::dialogDone,
			this, [this](QtMvvm::MessageConfig::StandardButton btn) {
		switch (btn) {
		case QtMvvm::MessageConfig::Retry:
		{
			auto mId = _id;
			_id = -1;
			setId(mId);
			break;
		}
		case QtMvvm::MessageConfig::Cancel:
			emit close();
			break;
		case QtMvvm::MessageConfig::Save:
			if(accept(true))
				emit close();
			break;
		default:
			break;
		}
	});
}

void AddAnimeViewModel::setLoading(bool loading)
{
	if (_loading == loading)
		return;
	_loading = loading;
	emit loadingChanged(loading);
	if(loading)
		setAcceptable(false);
}

void AddAnimeViewModel::setAcceptable(bool acceptable)
{
	if (_loading == acceptable)
		return;
	_acceptable = acceptable;
	emit acceptableChanged(acceptable);
}
