#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include "core_global.h"
#include "animestore.h"
#include "control.h"
#include "proxersettingscontrol.h"
#include "detailscontrol.h"
#include <objectlistmodel.h>
#include <settingscontrol.h>

class CORESHARED_EXPORT MainControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(GenericListModel<AnimeInfo>* animeModel READ animeModel CONSTANT)
	Q_PROPERTY(bool reloadingAnimes READ isReloadingAnimes NOTIFY reloadingAnimesChanged)

public:
	explicit MainControl(AnimeStore *store, QObject *parent = nullptr);

	GenericListModel<AnimeInfo>* animeModel() const;

	bool isReloadingAnimes() const;
	void updateLoadStatus(bool loading);

public slots:
	void reload();
	void showSettings();
	void showAbout();

	void uncheckAnime(int id);

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);

	void removeAnime(int id);

signals:
	void showStatus(QString message);
	void setProgress(int value, int max);

	void reloadingAnimesChanged(bool reloadingAnimes);

protected:
	void onShow() override;

private slots:
	void storeListLoaded(AnimeList list);

private:
	AnimeStore *store;
	GenericListModel<AnimeInfo> *model;
	bool _loading;

	DetailsControl *detailsControl;
	ProxerSettingsControl *settingsControl;

	AnimeInfo *infoFromId(int id) const;
	void createAddControl(int id = -1);
	void internalAddInfo(AnimeInfo *info);
};

Q_DECLARE_METATYPE(GenericListModel<AnimeInfo>*)

#endif // MAINCONTROL_H
