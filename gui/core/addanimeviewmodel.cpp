#include "addanimeviewmodel.h"
#include <QtMvvmCore/Messages>
#include <QtMvvmCore/CoreApp>
#include <proxerapi.h>
#include <apihelper.h>

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
	//WORKAROUND _infoClass(ProxerApi::factory().info().instance(this)),
	_infoClass((new ProxerApi(this))->info()),
	_loader(nullptr),
	_updater(nullptr),
	_nameModel(new QStringListModel(this)),
	_id(-1),
	_title(),
	_loading(false),
	_acceptable(false)
{
	_infoClass->setErrorTranslator(&ApiHelper::transformError);
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

QStringListModel *AddAnimeViewModel::nameModel() const
{
	return _nameModel;
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
			try {
				auto info = _store->load(_id);
				// can load -> already exists!
				QtMvvm::information(tr("Anime exists!"), tr("Anime does already exist. If you wanted to reset it, delete it first!"));
				emit resultReady(QVariant::fromValue(info));
			} catch(QtDataSync::NoDataException &) {
				AnimeInfo info {_id, _title};
				_store->save(info);
				_updater->checkForUpdates(info);
				emit resultReady(QVariant::fromValue(info));
			}
			return true;
		} catch(QException &e) {
			qCritical() << "Failed to save entry with id" << _id
						<< "and error:" << e.what();
			QtMvvm::critical(tr("Adding failed!"),
							 tr("Failed to add anime to underlying data storage."));
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

	//clear names etc, enter loading state
	_nameModel->setStringList({});
	setTitle({});
	updateLoading(true);

	// get the names
	auto repNames = _infoClass->getNames(_id);
	repNames->onSucceeded([this](int code, ProxerNames names){
		if(ApiHelper::testValid(code, names)) {
			QStringList allNames;
			for(auto name : names.data()) {
				if(name.eid() == _id)
					allNames.append(name.name());
			}
			_nameModel->setStringList(allNames);
			if(!allNames.isEmpty())
				setTitle(allNames.first());

			updateLoading(false);
			setAcceptable(true);
		} else
			error(names.message(), names.code(), QtRestClient::RestReply::FailureError);
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

void AddAnimeViewModel::error(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType)
{
	updateLoading(false);

	QtMvvm::MessageConfig config {
		QtMvvm::MessageConfig::TypeMessageBox,
		QtMvvm::MessageConfig::SubTypeCritical,
	};
	config.setTitle(tr("Network Error"))
			.setText(tr("<p>Unable to download data about the Anime from the server!</p>"
						"<p>%1</p>").arg(ApiHelper::formatError(errorString, errorCode, errorType)))
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

void AddAnimeViewModel::updateLoading(bool loading)
{
	_loading = loading;
	emit loadingChanged(loading);
	if(loading)
		setAcceptable(false);
}

void AddAnimeViewModel::setAcceptable(bool acceptable)
{
	if (_acceptable == acceptable)
		return;
	_acceptable = acceptable;
	emit acceptableChanged(acceptable);
}
