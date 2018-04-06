#include "mainviewmodel.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QtMvvmCore/Messages>
#include <QtMvvmDataSyncCore/DataSyncViewModel>

#include "aniremsettingsviewmodel.h"
#include "addanimeviewmodel.h"
#include "detailsviewmodel.h"

MainViewModel::MainViewModel(QObject *parent) :
	ViewModel(parent),
	_model(new QtDataSync::DataStoreModel(this)),
	_settings(nullptr),
	_updater(nullptr),
	_loading(false),
	_showNoChanges(false),
	_currentDetails()
{
	_model->setTypeId<AnimeInfo>();
}

QtDataSync::DataStoreModel *MainViewModel::animeModel() const
{
	return _model;
}

bool MainViewModel::isReloadingAnimes() const
{
	return _loading;
}

void MainViewModel::reload()
{
	if(!_updater->isUpdating()) {
		_showNoChanges = true;
		_updater->checkForUpdates();
	}
}

void MainViewModel::showSettings()
{
	show<AniremSettingsViewModel>();
}

void MainViewModel::showSync()
{
	show<QtMvvm::DataSyncViewModel>();
}

void MainViewModel::showAbout()
{
	QtMvvm::about(tr("A tool to passivly check for updates on seasons, for proxer.me."),
				  QStringLiteral("https://github.com/Skycoder42/SeasonProxer"),
				  tr("BSD-3-Clause"),
				  QStringLiteral("https://github.com/Skycoder42/AniRem/blob/master/LICENSE"),
				  tr("Skycoder42"));
}

void MainViewModel::showCaptcha()
{
	QDesktopServices::openUrl(QStringLiteral("https://proxer.me/misc/captcha"));
}

void MainViewModel::uncheckAnime(int id)
{
	auto info = infoFromId(id);
	if(info && info.hasNewSeasons()) {
		info.setAllUnchanged();
		_model->store()->save(info);
	}
}

void MainViewModel::itemAction(int id)
{
	auto info = infoFromId(id);

	if(info) {
		if(_settings->gui.uncheckEntries && info.hasNewSeasons()) {
			info.setAllUnchanged();
			_model->store()->save(info);
		}
		if(_settings->gui.openEntries)
			QDesktopServices::openUrl(info.relationsUrl());
	}
}

void MainViewModel::addAnime()
{
	show<AddAnimeViewModel>();
}

void MainViewModel::addAnimeFromClipboard()
{
	auto clipBoard = QGuiApplication::clipboard();
	auto text = clipBoard->text();

	auto id = -1;
	auto url = QUrl::fromUserInput(text);
	if(url.isValid() && !url.isRelative()) {
		if(url.host() == QStringLiteral("proxer.me")) {
			QStringList pathElements = url.path().split(QLatin1Char('/'), QString::SkipEmptyParts);
			if(pathElements.size() >= 2) {
				bool ok = false;
				id = pathElements[1].toInt(&ok);
				if(!ok)
					id = -1;
			}
		}
	}

	if(id == -1) {
		bool ok = false;
		id = text.toInt(&ok);
		if(!ok)
			id = -1;
	}

	if(id != -1)
		show<AddAnimeViewModel>(AddAnimeViewModel::params(id));
	else
		emit showStatus(tr("Clipboard does not contain a proxer url or an id"));
}

void MainViewModel::showDetails(int id)
{
	auto info = infoFromId(id);
	if(info) {
		if(_currentDetails)
			_currentDetails->setAnimeInfo(info);
		else
			show<DetailsViewModel>(DetailsViewModel::params(info, this));
	} else {
		if(_currentDetails)
			_currentDetails->clear();
	}
}

void MainViewModel::openUrl(int id)
{
	auto info = infoFromId(id);
	if(info)
		QDesktopServices::openUrl(info.relationsUrl());
}

void MainViewModel::removeAnime(int id)
{
	auto info = infoFromId(id);
	if(info) {
		emit showStatus(tr("Removed Anime: %1").arg(info.title()));
		_model->store()->remove<AnimeInfo>(info.id());
	}
}

void MainViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params)

	connect(_updater, &SeasonStatusLoader::started,
			this, &MainViewModel::updaterStarted);
	connect(_updater, &SeasonStatusLoader::completed,
			this, &MainViewModel::updaterDone);
	connect(_updater, &SeasonStatusLoader::updated,
			this, &MainViewModel::setProgress);

	if(_updater->isUpdating())
		updaterStarted();
}

void MainViewModel::setDetailsView(QPointer<DetailsViewModel> details)
{
	_currentDetails = std::move(details);
}

void MainViewModel::updaterStarted()
{
	_loading = true;
	emit reloadingAnimesChanged(_loading);
}

void MainViewModel::updaterDone(bool hasUpdates, const QString &error)
{
	_loading = false;
	emit reloadingAnimesChanged(_loading);

	if(!error.isNull())
		QtMvvm::critical(tr("Season check failed"), error);
	else if(hasUpdates)
		QtMvvm::information(tr("Season check completed"), tr("New Seasons are available!"));
	else if(_showNoChanges) {
		_showNoChanges = false;
		QtMvvm::information(tr("Season check completed"), tr("No seasons changed."));
	}
}

AnimeInfo MainViewModel::infoFromId(int id) const
{
	auto index = _model->idIndex(id);
	if(!index.isValid())
		return {};
	else
		return _model->object<AnimeInfo>(index);
}
