#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include "animeinfo.h"
#include "imageloader.h"
#include "ProxerApi/infoclass.h"

namespace Ui {
	class AddAnimeDialog;
}

class AddAnimeDialog : public QDialog
{
	Q_OBJECT

public:
	static AnimePtr createInfo(int id, QWidget *parent = nullptr);

private slots:
	void reloadAnime();

	void loadError(QString error);

private:
	Ui::AddAnimeDialog *ui;
	InfoClass *infoClass;
	QMovie *loadingMovie;
	int currentId;

	explicit AddAnimeDialog(QWidget *parent = nullptr);
	~AddAnimeDialog();
};

#endif // ADDANIMEDIALOG_H
