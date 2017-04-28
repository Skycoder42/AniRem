#ifndef DETAILSDOCKWIDGET_H
#define DETAILSDOCKWIDGET_H

#include <QDockWidget>
#include <detailscontrol.h>
#include <imageloader.h>

namespace Ui {
class DetailsDockWidget;
}

class DetailsDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	Q_INVOKABLE DetailsDockWidget(Control *mControl, QWidget *parent = nullptr);
	~DetailsDockWidget();

protected:
	void closeEvent(QCloseEvent *event) override;

private slots:
	void updateInfo();

	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &error);

private:
	DetailsControl *control;
	Ui::DetailsDockWidget *ui;

	ImageLoader *loader;
};

#endif // DETAILSDOCKWIDGET_H
