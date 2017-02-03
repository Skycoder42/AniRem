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
	qDeleteAll(ui->seasonStateWidget->findChildren<QLabel*>(QString(), Qt::FindDirectChildrenOnly));

	if(info) {
		setWindowTitle(tr("%1 Details").arg(info->title()));

		ui->previewLabel->setText(tr("<i>Loading preview image&#8230;</i>"));
		ImageLoader::instance()->loadImage(info->id(), [this](int id, QPixmap pm){
			if(control->animeInfo() && control->animeInfo()->id() == id)
				ui->previewLabel->setPixmap(pm);
		});

		for(auto it = info->seasonState().constBegin(); it != info->seasonState().constEnd(); it++) {
			auto content = new QLabel(ui->seasonStateWidget);
			content->setTextInteractionFlags(Qt::TextBrowserInteraction);
			if(it->second) {
				auto pal = content->palette();
				pal.setColor(QPalette::WindowText, QColor(0x8A, 0x0E, 0x0E));
				content->setPalette(pal);
				content->setText(tr("<b>%L1 (new)</b>").arg(it->first));
			} else
				content->setText(QLocale().toString(it->first));
			ui->formLayout->addRow(info->typeToString(it.key()) + tr(":"), content);
		}
	} else {
		setWindowTitle(tr("Anime Details"));
		ui->previewLabel->clear();
	}
}
