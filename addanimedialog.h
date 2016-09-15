#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include <QNetworkAccessManager>
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

	void imageLoaded();
	void pageLoaded();
	void tryCompleted();
	void abortError(const QString &error);

private:
	Ui::AddAnimeDialog *ui;
	QMovie *loadingMovie;

	QNetworkAccessManager *nam;
	QList<QNetworkReply*> activeReplies;

	int currentId;
	QPixmap cPixmap;
	QString cTitle;

	explicit AddAnimeDialog(QWidget *parent = nullptr);
	~AddAnimeDialog();
};

#endif // ADDANIMEDIALOG_H
