#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include "animestore.h"
#include "control.h"
#include <objectlistmodel.h>

class MainControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(GenericListModel<AnimeInfo>* animeModel READ animeModel CONSTANT)

public:
	explicit MainControl(AnimeStore *store, QObject *parent = nullptr);

	GenericListModel<AnimeInfo>* animeModel() const;

public slots:
	void reload();

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);

signals:
	void showStatus(QString message, bool permanent = false);
	void setProgress(int value, int max);

	void updateLoadStatus(bool isFinished);

private slots:
	void setAnimeList(AnimeList list);

protected:
	void onShow();

private:
	AnimeStore *store;
	GenericListModel<AnimeInfo> *model;
};

#endif // MAINCONTROL_H
