#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include "animestore.h"
#include "control.h"

class MainControl : public Control
{
	Q_OBJECT

	//TODO model property
public:
	explicit MainControl(AnimeStore *store, QObject *parent = nullptr);

public slots:
	void reload();

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);

signals:
	void showStatus(QString message, bool permanent = false);
	void setProgress(int value, int max);

	void updateLoadStatus(bool isFinished);
	//TODO AnimeModel *model;

protected:
	void onShow();
};

#endif // MAINCONTROL_H
