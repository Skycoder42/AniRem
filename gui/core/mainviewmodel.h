#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <QtDataSync/DataStoreModel>
#include <syncedsettings.h>

#include "animeinfo.h"

class MainViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(QtDataSync::DataStoreModel* animeModel READ animeModel CONSTANT)
	Q_PROPERTY(bool reloadingAnimes READ isReloadingAnimes NOTIFY reloadingAnimesChanged)

	QTMVVM_INJECT_PROP(SyncedSettings*, settings, _settings)

public:
	Q_INVOKABLE explicit MainViewModel(QObject *parent = nullptr);

	QtDataSync::DataStoreModel* animeModel() const;

	bool isReloadingAnimes() const;
	void updateLoadStatus(bool loading);

public slots:
	void reload();
	void showSettings();
	void showSync();
	void showAbout();
	void showCaptcha();

	void uncheckAnime(int id);
	void itemAction(int id);

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);
	void openUrl(int id);

	void removeAnime(int id);

signals:
	void showStatus(const QString &message);
	void setProgress(int value, int max);

	void reloadingAnimesChanged(bool reloadingAnimes);

protected:

private:
	QtDataSync::DataStoreModel *_model;
	SyncedSettings *_settings;
	bool _loading;

	AnimeInfo infoFromId(int id) const;
};

#endif // MAINVIEWMODEL_H
