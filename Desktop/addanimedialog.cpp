#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QDebug>
#include <QtRestClient>
#include "core.h"
#include "dialogmaster.h"
using namespace QtRestClient;

AddAnimeDialog::AddAnimeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAnimeDialog),
	infoClass(new InfoClass(this)),
	loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this)),
	currentId(-1)
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this, true);
	ui->proxerIDLineEdit->setValidator(new QIntValidator(0, INT_MAX, ui->proxerIDLineEdit));//TODO combobox with all names
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	connect(ui->proxerIDLineEdit, &QLineEdit::editingFinished,
			this, &AddAnimeDialog::reloadAnime);

	connect(infoClass, &InfoClass::apiError,
			this, &AddAnimeDialog::loadError);
}

AddAnimeDialog::~AddAnimeDialog()
{
	delete ui;
}

AnimeInfo *AddAnimeDialog::createInfo(int id, QWidget *parent)
{
	AddAnimeDialog dialog(parent);

	if(id != -1) {
		dialog.ui->proxerIDLineEdit->setText(QString::number(id));
		dialog.reloadAnime();
	}

	if(dialog.exec() == QDialog::Accepted)
		return new AnimeInfo(dialog.currentId, dialog.ui->titleLineEdit->text());//TODO parent
	else
		return {};
}

void AddAnimeDialog::reloadAnime()
{
	if(!ui->proxerIDLineEdit->isEnabled() ||
	   !ui->titleLineEdit->isReadOnly())
		return;

	if(!ui->proxerIDLineEdit->text().isEmpty()) {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		ui->proxerIDLineEdit->setEnabled(false);
		ui->previewLabel->setScaledContents(false);
		ui->previewLabel->setMovie(loadingMovie);
		loadingMovie->start();

		currentId = QLocale().toInt(ui->proxerIDLineEdit->text());
		infoClass->getEntry(currentId)->onSucceeded([this](RestReply*, int code, ProxerEntry *entry){
			if(infoClass->testValid(code, entry)) {
				Q_ASSERT(entry->data->id == currentId);
				ui->proxerIDLineEdit->setEnabled(true);
				ui->titleLineEdit->setText(entry->data->name);
				ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			}
			entry->deleteLater();
		});

		ImageLoader::instance()->loadImage(currentId, [this](int id, QPixmap pm){
			Q_ASSERT(id == currentId);
			ui->previewLabel->setScaledContents(true);
			ui->previewLabel->setPixmap(pm);
			loadingMovie->stop();
		});
	}
}

void AddAnimeDialog::loadError(QString error)
{
	ui->proxerIDLineEdit->setEnabled(true);
	ui->titleLineEdit->clear();
	ui->previewLabel->setScaledContents(false);
	ui->previewLabel->setText(QStringLiteral("<i>preview</i>"));
	loadingMovie->stop();

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
		reloadAnime();
		break;
	case QMessageBox::Cancel:
		reject();
		break;
	case QMessageBox::Apply:
		ui->proxerIDLineEdit->setEnabled(true);
		ui->titleLineEdit->setReadOnly(false);
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		break;
	default:
		break;
	}
}
