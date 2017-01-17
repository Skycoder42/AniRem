#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QDebug>
#include <QtRestClient>
#include "core.h"
#include "dialogmaster.h"
using namespace QtRestClient;

AddAnimeDialog::AddAnimeDialog(Control *mControl, QWidget *parent) :
	QDialog(parent),
	control(static_cast<AddAnimeControl*>(mControl)),
	ui(new Ui::AddAnimeDialog),
	loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this)),
	currentPixmap(),
	pmLoading(false)
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this, true);
	ui->proxerIDLineEdit->setValidator(new QIntValidator(0, INT_MAX, ui->proxerIDLineEdit));

	connect(ui->proxerIDLineEdit, &QLineEdit::editingFinished,
			this, &AddAnimeDialog::reloadAnime);

	//control
	connect(control, &AddAnimeControl::idChanged,
			this, &AddAnimeDialog::idChanged);
	connect(control, &AddAnimeControl::titleChanged,
			ui->titleLineEdit, &QLineEdit::setText);
	connect(control, &AddAnimeControl::loadingChanged,
			this, &AddAnimeDialog::loadingChanged);
	connect(control, &AddAnimeControl::acceptableChanged,
			ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::setEnabled);
	connect(control, &AddAnimeControl::loadError,
			this, &AddAnimeDialog::loadError);

	idChanged(control->id());//TODO mvvm: add "init-connect" that does both
	ui->titleLineEdit->setText(control->title());
	loadingChanged(control->isLoading());
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(control->isAcceptable());
}

AddAnimeDialog::~AddAnimeDialog()
{
	delete ui;
}

void AddAnimeDialog::accept()
{
	control->accept(!ui->titleLineEdit->isReadOnly());
	QDialog::accept();
}

void AddAnimeDialog::idChanged(int id)
{
	if(id == -1) {
		ui->proxerIDLineEdit->clear();
		currentPixmap = QPixmap();
		pmLoading = false;
		updatePm();
	} else {
		ui->proxerIDLineEdit->setText(QString::number(id));

		currentPixmap = QPixmap();
		pmLoading = true;
		updatePm();
		ImageLoader::instance()->loadImage(id, [=](int id, QPixmap pm){
			if(control->id() == id) {
				currentPixmap = pm;
				pmLoading = false;
				updatePm();
			}
		});
	}
}

void AddAnimeDialog::loadingChanged(bool loading)
{
	ui->proxerIDLineEdit->setEnabled(!loading);
	updatePm();
}

void AddAnimeDialog::reloadAnime()
{
	if(!ui->proxerIDLineEdit->isEnabled() ||
	   !ui->titleLineEdit->isReadOnly())
		return;

	if(!ui->proxerIDLineEdit->text().isEmpty()) {
		control->setId(QLocale().toInt(ui->proxerIDLineEdit->text()));
	}
}

void AddAnimeDialog::loadError(QString error)
{
	pmLoading = false;
	updatePm();

	auto config = DialogMaster::createCritical(QString(), this);
	config.title = tr("Network Error");
	config.text = tr("Unable to download data about the Anime from the server!");
	config.details = error;
	config.buttons = QMessageBox::Retry | QMessageBox::Cancel | QMessageBox::Apply;
	config.defaultButton = QMessageBox::Retry;
	config.escapeButton = QMessageBox::Cancel;
	config.buttonTexts.insert(QMessageBox::Apply, tr("Save Anyway"));
	switch (DialogMaster::messageBox(config)) {
	case QMessageBox::Retry:
		control->retry();
		break;
	case QMessageBox::Cancel:
		reject();
		break;
	case QMessageBox::Apply:
		control->accept(true);
		QDialog::accept();
		break;
	default:
		break;
	}
}

void AddAnimeDialog::updatePm()
{
	if(control->isLoading() || pmLoading) {
		loadingMovie->start();
		ui->previewLabel->clear();
		ui->previewLabel->setScaledContents(false);
		ui->previewLabel->setMovie(loadingMovie);
	} else {
		if(!currentPixmap.isNull()) {
			ui->previewLabel->clear();
			ui->previewLabel->setScaledContents(true);
			ui->previewLabel->setPixmap(currentPixmap);
		} else {
			ui->previewLabel->clear();
			ui->previewLabel->setText(QStringLiteral("<i>preview</i>"));
		}
		loadingMovie->stop();
	}
}
