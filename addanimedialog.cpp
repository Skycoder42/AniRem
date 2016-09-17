#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QtNetwork>
#include "dialogmaster.h"

AddAnimeDialog::AddAnimeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAnimeDialog),
	connector(new ProxerConnector(this)),
	loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this)),
	currentId(-1)
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this, true);
	this->ui->proxerIDLineEdit->setValidator(new QIntValidator(0, INT_MAX, this->ui->proxerIDLineEdit));
	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	connect(this->ui->proxerIDLineEdit, &QLineEdit::editingFinished,
			this, &AddAnimeDialog::reloadAnime);

	connect(this->connector, &ProxerConnector::metaDataLoaded,
			this, &AddAnimeDialog::loaded);
	connect(this->connector, &ProxerConnector::networkError,
			this, &AddAnimeDialog::loadError);
}

AddAnimeDialog::~AddAnimeDialog()
{
	delete ui;
}

AnimeInfo AddAnimeDialog::createInfo(int id, QWidget *parent)
{
	AddAnimeDialog dialog(parent);

	if(id != -1) {
		dialog.ui->proxerIDLineEdit->setText(QString::number(id));
		dialog.reloadAnime();
	}

	if(dialog.exec() == QDialog::Accepted) {
		AnimeInfo info;
		info.id = dialog.currentId;
		info.title = dialog.ui->titleLineEdit->text();
		if(dialog.ui->previewLabel->pixmap())
			info.previewImage = *(dialog.ui->previewLabel->pixmap());
		return info;
	} else
		return AnimeInfo();
}

void AddAnimeDialog::reloadAnime()
{
	if(!this->ui->proxerIDLineEdit->isEnabled() ||
	   !this->ui->titleLineEdit->isReadOnly())
		return;

	if(!this->ui->proxerIDLineEdit->text().isEmpty()) {
		this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		this->ui->proxerIDLineEdit->setEnabled(false);
		this->ui->previewLabel->setScaledContents(false);
		this->ui->previewLabel->setMovie(this->loadingMovie);
		this->loadingMovie->start();

		this->currentId = QLocale().toInt(this->ui->proxerIDLineEdit->text());
		this->connector->loadMetaData(this->currentId);
	}
}

void AddAnimeDialog::loaded(int id, QString title, QPixmap preview)
{
	if(id == this->currentId) {
		this->ui->proxerIDLineEdit->setEnabled(true);
		this->ui->titleLineEdit->setText(title);
		this->ui->previewLabel->setScaledContents(true);
		this->ui->previewLabel->setPixmap(preview);
		this->loadingMovie->stop();
		this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}

void AddAnimeDialog::loadError(QString error)
{
	this->ui->proxerIDLineEdit->setEnabled(true);
	this->ui->titleLineEdit->clear();
	this->ui->previewLabel->setScaledContents(false);
	this->ui->previewLabel->setText(QStringLiteral("<i>preview</i>"));
	this->loadingMovie->stop();

	auto config = DialogMaster::createCritical(QString(), this);
	config.title = tr("Network Error");
	config.text = tr("Unable to download data about the Anime from the server.<br>"
					 "You can either cancel or add the anime without details. With the "
					 "next update check the data will be completed.");
	config.details = error;
	config.buttons = QMessageBox::Retry | QMessageBox::Cancel | QMessageBox::Apply;
	config.defaultButton = QMessageBox::Retry;
	config.escapeButton = QMessageBox::Cancel;
	config.buttonTexts.insert(QMessageBox::Apply, tr("Manually"));
	switch (DialogMaster::messageBox(config)) {
	case QMessageBox::Retry:
		this->reloadAnime();
		break;
	case QMessageBox::Cancel:
		this->reject();
		break;
	case QMessageBox::Apply:
		this->ui->proxerIDLineEdit->setEnabled(true);
		this->ui->titleLineEdit->setReadOnly(false);
		this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		break;
	default:
		break;
	}
}
