#ifndef DETAILSDOCKWIDGET_H
#define DETAILSDOCKWIDGET_H

#include <QDockWidget>
#include <detailsviewmodel.h>

namespace Ui {
class DetailsDockWidget;
}

class DetailsDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	Q_INVOKABLE DetailsDockWidget(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~DetailsDockWidget() override;

protected:
	void closeEvent(QCloseEvent *event) override;

private slots:
	void updateInfo();

	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &error);

private:
	DetailsViewModel *_viewModel;
	Ui::DetailsDockWidget *_ui;
};

#endif // DETAILSDOCKWIDGET_H
