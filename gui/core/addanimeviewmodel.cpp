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
	_infoClass(ProxerApi::factory().info().instance(this)),
	_loader(nullptr),
	_updater(nullptr),
	_nameModel(new QStringListModel(this)),
	_id(-1),
	_title(),
	_loadingEntry(false),
	_loadingNames(false),
	_acceptable(false)
{}

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
	return _loadingEntry || _loadingNames;
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
				_store->load(_id);
				// can load -> already exists!
				QtMvvm::information(tr("Anime exists!"), tr("Anime does already exist. If you wanted to reset it, delete it first!"));
			} catch(QtDataSync::NoDataException &) {
				AnimeInfo info {_id, _title};
				_store->save(info);
				_updater->checkForUpdates(info);
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
	_loadingEntry = true;
	_loadingNames = true;
	updateLoading();

	// get the entry
	auto repEntry = _infoClass->getEntry(_id);
	repEntry->onSucceeded([this](int code, ProxerEntry entry){
		_loadingEntry = false;
		updateLoading();

		if(ApiHelper::testValid(code, entry)) {
			if(entry.data().id() == _id) {
				_title = entry.data().name();
				updateNames({});
				setAcceptable(true);
			}
		} else
		   error(entry.message(), entry.code(), QtRestClient::RestReply::FailureError);
	});
	repEntry->onError([this](QString e, int c, QtRestClient::RestReply::ErrorType t){
		_loadingEntry = false;
		updateLoading();
		error(e, c, t);
	});

	// get the names
	auto repNames = _infoClass->getNames(_id);
	repNames->onSucceeded([this](int code, ProxerNames names){
		_loadingNames = false;
		updateLoading();

		if(ApiHelper::testValid(code, names)) {
			QStringList allNames;
			for(auto name : names.data()) {
				if(name.eid() == _id)
					allNames.append(name.name());
			}
			updateNames(allNames);
		} else
		   ApiHelper::formatError(names.message(), names.code(), QtRestClient::RestReply::FailureError); //only log error, no dialog etc
	});
	repNames->onError([this](QString e, int c, QtRestClient::RestReply::ErrorType t){
		_loadingNames = false;
		updateLoading();
		ApiHelper::formatError(e, c, t); //only log error, no dialog etc
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

void AddAnimeViewModel::updateLoading()
{
	emit loadingChanged(isLoading());
	if(isLoading())
		setAcceptable(false);
}

void AddAnimeViewModel::updateNames(const QStringList &allNames)
{
	QStringList mNames;
	if(allNames.isEmpty())
		mNames = _nameModel->stringList();
	else
		mNames = allNames;

	if(!_title.isEmpty() && !mNames.contains(_title))
		mNames.prepend(_title);
	_nameModel->setStringList(mNames);

	emit titleChanged(_title);
}

void AddAnimeViewModel::setAcceptable(bool acceptable)
{
	if (_acceptable == acceptable)
		return;
	_acceptable = acceptable;
	emit acceptableChanged(acceptable);
}
