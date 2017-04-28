#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include <addanimecontrol.h>
#include <animeinfo.h>
#include <imageloader.h>

namespace Ui {
	class AddAnimeDialog;
}

class AddAnimeDialog : public QDialog
{
	Q_OBJECT

public:
	Q_INVOKABLE AddAnimeDialog(Control *mControl, QWidget *parent = nullptr);
	~AddAnimeDialog();

	void accept() override;

private slots:
	void idChanged(int id);
	void loadingChanged(bool loading);

	void reloadAnime();

	void loadError(QString error);

	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &error);

private:
	AddAnimeControl *control;
	Ui::AddAnimeDialog *ui;
	ImageLoader *loader;

	QMovie *loadingMovie;
	QPixmap currentPixmap;
	bool pmLoading;

	void updatePm();
};

#endif // ADDANIMEDIALOG_H
