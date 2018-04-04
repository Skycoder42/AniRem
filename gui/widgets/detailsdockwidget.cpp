#include "detailsdockwidget.h"
#include "ui_detailsdockwidget.h"

#include <QCloseEvent>
#include <QMainWindow>

DetailsDockWidget::DetailsDockWidget(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDockWidget(parent),
	_viewModel(static_cast<DetailsViewModel*>(viewModel)),
	_ui(new Ui::DetailsDockWidget)
{
	_ui->setupUi(this);
	setMinimumWidth(_ui->previewLabel->minimumWidth() +
					8 +
					style()->pixelMetric(QStyle::PM_ScrollBarExtent));

	connect(_viewModel->imageLoader(), &ImageLoader::imageLoaded,
			this, &DetailsDockWidget::imageLoaded);
	connect(_viewModel->imageLoader(), &ImageLoader::imageLoadFailed,
			this, &DetailsDockWidget::imageLoadFailed);

	connect(_viewModel, &DetailsViewModel::animeInfoChanged,
			this, &DetailsDockWidget::updateInfo);
	updateInfo();

	QMetaObject::invokeMethod(parent, "addDockAction",
							  Q_ARG(QAction*, toggleViewAction()));
}

DetailsDockWidget::~DetailsDockWidget()
{
	delete _ui;
}

void DetailsDockWidget::closeEvent(QCloseEvent *event)
{
	event->ignore();
	hide();
}

void DetailsDockWidget::updateInfo()
{
	auto info = _viewModel->animeInfo();
	if(info) {
		setWindowTitle(tr("%1 Details").arg(info.title()));

		_ui->previewLabel->setText(tr("<i>Loading preview image&#8230;</i>"));
		_viewModel->imageLoader()->loadImage(info.id());
		_ui->detailsLabel->setText(_viewModel->detailsText());
	} else {
		setWindowTitle(tr("Anime Details"));
		_ui->previewLabel->clear();
		_ui->detailsLabel->clear();
	}
}

void DetailsDockWidget::imageLoaded(int id, const QImage &image)
{
	if(_viewModel->animeInfo() && _viewModel->animeInfo().id() == id)
		_ui->previewLabel->setPixmap(QPixmap::fromImage(image));
}

void DetailsDockWidget::imageLoadFailed(int id, const QString &error)
{
	if(_viewModel->animeInfo() && _viewModel->animeInfo().id() == id) {
		qWarning().noquote() << error;
		_ui->previewLabel->setText(tr("No image"));
	}
}
