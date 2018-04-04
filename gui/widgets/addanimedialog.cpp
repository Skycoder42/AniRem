#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QtMvvmCore/Binding>
#include <dialogmaster.h>

AddAnimeDialog::AddAnimeDialog(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDialog(parent),
	_viewModel(static_cast<AddAnimeViewModel*>(viewModel)),
	_ui(new Ui::AddAnimeDialog),
	_loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this)),
	_currentPixmap(),
	_pmState(PmCleared)
{
	_ui->setupUi(this);
	DialogMaster::masterDialog(this, true);
	_ui->proxerIDLineEdit->setValidator(new QIntValidator(0, INT_MAX, _ui->proxerIDLineEdit));

	connect(_viewModel->imageLoader(), &ImageLoader::imageLoaded,
			this, &AddAnimeDialog::imageLoaded,
			Qt::QueuedConnection);
	connect(_viewModel->imageLoader(), &ImageLoader::imageLoadFailed,
			this, &AddAnimeDialog::imageLoadFailed,
			Qt::QueuedConnection);

	connect(_ui->proxerIDLineEdit, &QLineEdit::editingFinished,
			this, &AddAnimeDialog::uiIdChanged);

	//control
	QtMvvm::bind(_viewModel, "id",
				 this, "uiId");
	QtMvvm::bind(_viewModel, "title",
				 _ui->titleLineEdit, "text",
				 QtMvvm::Binding::OneWayToView);
	QtMvvm::bind(_viewModel, "acceptable",
				 _ui->buttonBox->button(QDialogButtonBox::Ok), "enabled",
				 QtMvvm::Binding::OneWayToView);

	connect(_viewModel, &AddAnimeViewModel::loadingChanged,
			this, &AddAnimeDialog::loadingChanged);
	loadingChanged(_viewModel->isLoading());

	connect(_viewModel, &AddAnimeViewModel::close,
			this, &AddAnimeDialog::reject);
}

AddAnimeDialog::~AddAnimeDialog()
{
	delete _ui;
}

void AddAnimeDialog::accept()
{
	if(_viewModel->accept(!_ui->titleLineEdit->isReadOnly()))
		QDialog::accept();
}

int AddAnimeDialog::uiId() const
{
	auto ok = false;
	auto res = QLocale().toInt(_ui->proxerIDLineEdit->text(), &ok);
	if(ok)
		return res;
	else
		return -1;
}

void AddAnimeDialog::setUiId(int id)
{
	_pmState = PmCleared;
	if(id == -1) {
		_ui->proxerIDLineEdit->clear();
		updatePm();
	} else {
		_ui->proxerIDLineEdit->setText(QString::number(id));
		_pmState = PmLoading;
		updatePm();
		_viewModel->imageLoader()->loadImage(id);
	}
}

void AddAnimeDialog::loadingChanged(bool loading)
{
	_ui->proxerIDLineEdit->setEnabled(!loading);
	updatePm();
}

void AddAnimeDialog::imageLoaded(int id, const QImage &image)
{
	if(_viewModel->id() == id) {
		_currentPixmap = QPixmap::fromImage(image);
		_pmState = PmLoaded;
		updatePm();
	}
}

void AddAnimeDialog::imageLoadFailed(int id, const QString &error)
{
	if(_viewModel->id() == id) {
		qWarning().noquote() << error;
		_pmState = PmError;
		updatePm();
	}
}

void AddAnimeDialog::updatePm()
{
	if(_viewModel->isLoading() || _pmState == PmLoading) {
		_loadingMovie->start();
		_ui->previewLabel->clear();
		_ui->previewLabel->setScaledContents(false);
		_ui->previewLabel->setMovie(_loadingMovie);
	} else {
		switch(_pmState) {
		case AddAnimeDialog::PmCleared:
			_currentPixmap = QPixmap();
			_ui->previewLabel->clear();
			_ui->previewLabel->setText(tr("<i>Preview</i>"));
			break;
		case AddAnimeDialog::PmLoaded:
			_ui->previewLabel->clear();
			_ui->previewLabel->setScaledContents(true);
			_ui->previewLabel->setPixmap(_currentPixmap);
			break;
		case AddAnimeDialog::PmError:
			_currentPixmap = QPixmap();
			_ui->previewLabel->clear();
			_ui->previewLabel->setText(tr("<i>No image</i>"));
			break;
		default:
			Q_UNREACHABLE();
			break;
		}
		_loadingMovie->stop();
	}
}
