#include "detailsdockwidget.h"
#include "ui_detailsdockwidget.h"

#include <QCloseEvent>
#include <QMainWindow>

DetailsDockWidget::DetailsDockWidget(Control *mControl, QWidget *parent) :
	QDockWidget(parent),
	control(static_cast<DetailsControl*>(mControl)),
	ui(new Ui::DetailsDockWidget),
	loader(new ImageLoader(this))
{
	ui->setupUi(this);
	setMinimumWidth(ui->previewLabel->minimumWidth() +
					2 +
					style()->pixelMetric(QStyle::PM_ScrollBarExtent));

	connect(loader, &ImageLoader::imageLoaded,
			this, &DetailsDockWidget::imageLoaded);
	connect(loader, &ImageLoader::imageLoadFailed,
			this, &DetailsDockWidget::imageLoadFailed);

	connect(control, &DetailsControl::animeInfoChanged,
			this, &DetailsDockWidget::updateInfo);
	updateInfo();

	QMetaObject::invokeMethod(parent, "addDockAction",
							  Q_ARG(QAction*, toggleViewAction()));
}

DetailsDockWidget::~DetailsDockWidget()
{
	delete ui;
}

void DetailsDockWidget::closeEvent(QCloseEvent *event)
{
	event->ignore();
	hide();
}

void DetailsDockWidget::updateInfo()
{
	auto info = control->animeInfo();
	if(info) {
		setWindowTitle(tr("%1 Details").arg(info->title()));

		ui->previewLabel->setText(tr("<i>Loading preview image&#8230;</i>"));
		loader->loadImage(info->id());
		ui->detailsLabel->setText(control->detailsText());
	} else {
		setWindowTitle(tr("Anime Details"));
		ui->previewLabel->clear();
		ui->detailsLabel->clear();
	}
}

void DetailsDockWidget::imageLoaded(int id, const QImage &image)
{
	if(control->animeInfo() && control->animeInfo()->id() == id)
		ui->previewLabel->setPixmap(QPixmap::fromImage(image));
}

void DetailsDockWidget::imageLoadFailed(int id, const QString &error)
{
	if(control->animeInfo() && control->animeInfo()->id() == id)
		ui->previewLabel->setText(error);
}
