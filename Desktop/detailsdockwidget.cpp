#include "detailsdockwidget.h"
#include "ui_detailsdockwidget.h"
#include "imageloader.h"

#include <QMainWindow>

DetailsDockWidget::DetailsDockWidget(Control *mControl, QWidget *parent) :
	QDockWidget(parent),
	control(static_cast<DetailsControl*>(mControl)),
	ui(new Ui::DetailsDockWidget)
{
	ui->setupUi(this);
	setMinimumWidth(ui->previewLabel->minimumWidth() +
					2 +
					style()->pixelMetric(QStyle::PM_ScrollBarExtent));

	connect(control, &DetailsControl::animeInfoChanged,
			this, &DetailsDockWidget::updateInfo);
	updateInfo(control->animeInfo());
}

DetailsDockWidget::~DetailsDockWidget()
{
	delete ui;
}

void DetailsDockWidget::updateInfo(AnimeInfo *info)
{
	if(info) {
		setWindowTitle(tr("%1 Details").arg(info->title()));

		ui->previewLabel->setText(tr("<i>Loading preview image&#8230;</i>"));
		ImageLoader::instance()->loadImage(info->id(), [this](int id, QPixmap pm){
			if(control->animeInfo() && control->animeInfo()->id() == id)
				ui->previewLabel->setPixmap(pm);
		});

		ui->detailsLabel->setText(control->detailsText());
	} else {
		setWindowTitle(tr("Anime Details"));
		ui->previewLabel->clear();
		ui->detailsLabel->clear();
	}
}
