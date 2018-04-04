#include "mainviewmodel.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QtMvvmCore/Messages>
#include <QtMvvmDataSyncCore/DataSyncViewModel>

#include "datasyncsettingsviewmodel.h"
#include "addanimeviewmodel.h"

MainViewModel::MainViewModel(QObject *parent) :
	ViewModel(parent),
	_model(new QtDataSync::DataStoreModel(this)),
	_settings(nullptr),
	_loading(false)
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

void MainViewModel::updateLoadStatus(bool loading)
{
	if(_loading == loading)
		return;

	_loading = loading;
	emit reloadingAnimesChanged(loading);
}

void MainViewModel::reload()
{
	Q_UNIMPLEMENTED();
}

void MainViewModel::showSettings()
{
	show<DataSyncSettingsViewModel>();
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
		;//TODO show
	else
		emit showStatus(tr("Clipboard does not contain a proxer url or an id"));
}

void MainViewModel::showDetails(int id)
{
	auto info = infoFromId(id);
	if(info) {
		//TODO show
	} else {
		//TODO close
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

AnimeInfo MainViewModel::infoFromId(int id) const
{
	auto index = _model->idIndex(id);
	if(!index.isValid())
		return {};
	else
		return _model->object<AnimeInfo>(index);
}
