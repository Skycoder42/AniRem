#ifndef DETAILSDOCKWIDGET_H
#define DETAILSDOCKWIDGET_H

#include <QDockWidget>
#include <detailscontrol.h>

namespace Ui {
class DetailsDockWidget;
}

class DetailsDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	Q_INVOKABLE DetailsDockWidget(Control *mControl, QWidget *parent = nullptr);
	~DetailsDockWidget();

private slots:
	void updateInfo(AnimeInfo *info);

private:
	DetailsControl *control;
	Ui::DetailsDockWidget *ui;
};

#endif // DETAILSDOCKWIDGET_H
