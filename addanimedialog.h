#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include "animeinfo.h"

namespace Ui {
	class AddAnimeDialog;
}

class AddAnimeDialog : public QDialog
{
	Q_OBJECT

public:
	static AnimeInfo createInfo(int id, QWidget *parent = nullptr);

private slots:
	void reloadAnime();

private:
	Ui::AddAnimeDialog *ui;
	QMovie *loadingMovie;

	explicit AddAnimeDialog(QWidget *parent = nullptr);
	~AddAnimeDialog();
};

#endif // ADDANIMEDIALOG_H
