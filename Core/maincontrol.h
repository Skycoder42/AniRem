#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include "core_global.h"
#include "animestore.h"
#include "control.h"
#include <objectlistmodel.h>

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

	void uncheckAnime(int index);

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);

	void removeAnime(int index);

signals:
	void showStatus(QString message);
	void setProgress(int value, int max);

	void reloadingAnimesChanged(bool reloadingAnimes);

private slots:
	void storeListLoaded(AnimeList list);

protected:
	void onShow() override;

private:
	AnimeStore *store;
	GenericListModel<AnimeInfo> *model;
	bool _loading;

	void createAddControl(int id = -1);
	void internalAddInfo(AnimeInfo *info);
};

Q_DECLARE_METATYPE(GenericListModel<AnimeInfo>*)

#endif // MAINCONTROL_H
