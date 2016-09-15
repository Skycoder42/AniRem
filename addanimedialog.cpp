#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>

AddAnimeDialog::AddAnimeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAnimeDialog),
	loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this))
{
	ui->setupUi(this);
	this->ui->proxerIDLineEdit->setValidator(new QIntValidator(0, INT_MAX, this->ui->proxerIDLineEdit));
	this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	connect(this->ui->proxerIDLineEdit, &QLineEdit::editingFinished,
			this, &AddAnimeDialog::reloadAnime);
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
		info.id = dialog.ui->proxerIDLineEdit->text().toInt();
		info.title = dialog.ui->titleLineEdit->text();
		if(dialog.ui->previewLabel->pixmap())
			info.previewImage = *dialog.ui->previewLabel->pixmap();
		return info;
	} else
		return AnimeInfo();
}

void AddAnimeDialog::reloadAnime()
{
	this->ui->proxerIDLineEdit->setEnabled(false);
	this->ui->previewLabel->setMovie(this->loadingMovie);
	this->loadingMovie->start();
}
