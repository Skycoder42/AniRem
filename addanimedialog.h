#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include <QNetworkAccessManager>
#include "animeinfo.h"
#include "proxerconnector.h"

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

	void loaded(const AnimeInfo &info);
	void loadError(QString error);

private:
	Ui::AddAnimeDialog *ui;
	ProxerConnector *connector;
	QMovie *loadingMovie;
	int currentId;

	explicit AddAnimeDialog(QWidget *parent = nullptr);
	~AddAnimeDialog();
};

#endif // ADDANIMEDIALOG_H
