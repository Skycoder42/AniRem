#include "addanimedialog.h"
#include "ui_addanimedialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QtNetwork>
#include "dialogmaster.h"

AddAnimeDialog::AddAnimeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddAnimeDialog),
	loadingMovie(new QMovie(QStringLiteral(":/animations/loading.gif"), "gif", this)),
	nam(new QNetworkAccessManager(this)),
	activeReplies(),
	currentId(-1),
	cPixmap(),
	cTitle()
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this, true);
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
		info.id = dialog.currentId;
		info.title = dialog.cTitle;
		info.previewImage = dialog.cPixmap;
		return info;
	} else
		return AnimeInfo();
}

void AddAnimeDialog::reloadAnime()
{
	if(!this->ui->proxerIDLineEdit->isEnabled())
		return;

	if(!this->ui->proxerIDLineEdit->text().isEmpty()) {
		this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		this->ui->proxerIDLineEdit->setEnabled(false);
		this->ui->previewLabel->setScaledContents(false);
		this->ui->previewLabel->setMovie(this->loadingMovie);
		this->loadingMovie->start();

		this->currentId = QLocale().toInt(this->ui->proxerIDLineEdit->text());

		QNetworkRequest pageRequest(QUrl(QStringLiteral("http://proxer.me/info/%1/details").arg(this->currentId)));
		pageRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
		auto pageReply = this->nam->get(pageRequest);
		this->activeReplies.append(pageReply);
		connect(pageReply, &QNetworkReply::finished,
				this, &AddAnimeDialog::pageLoaded);

		QNetworkRequest iconRequest(QUrl(QStringLiteral("http://cdn.proxer.me/cover/%1.jpg").arg(this->currentId)));
		iconRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
		auto iconReply = this->nam->get(iconRequest);
		this->activeReplies.append(iconReply);
		connect(iconReply, &QNetworkReply::finished,
				this, &AddAnimeDialog::imageLoaded);
	}
}

void AddAnimeDialog::imageLoaded()
{
	auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
	Q_ASSERT(reply);

	if(this->activeReplies.removeOne(reply)) {
		if(reply->error() == QNetworkReply::NoError) {
			this->cPixmap.loadFromData(reply->readAll(), "jpg");
			this->tryCompleted();
		} else if(reply->error() != QNetworkReply::OperationCanceledError)
			this->abortError(reply->errorString());
	}

	reply->deleteLater();
}

void AddAnimeDialog::pageLoaded()
{
	auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
	Q_ASSERT(reply);

	if(this->activeReplies.removeOne(reply)) {
		if(reply->error() == QNetworkReply::NoError) {
			auto rawData = QString::fromUtf8(reply->readAll());

			//check if "hentai"
			if(rawData.contains(QStringLiteral("<title>Info</title>")))
				this->abortError(tr("This tool can't add protected animes (e.g. Hentais)"));
			else {
				//find the label
				auto start = rawData.indexOf(QStringLiteral("<td><b>Original Titel</b></td>"));
				auto beginTitle = rawData.indexOf(QStringLiteral("<td>"), start + 4) + 4;
				auto endTitle = rawData.indexOf(QStringLiteral("</td>"), beginTitle);
				if(start != -1 &&
				   beginTitle != -1 &&
				   endTitle != -1 &&
				   endTitle > beginTitle) {
					this->cTitle = rawData.mid(beginTitle, endTitle - beginTitle);
					this->tryCompleted();
				} else
					this->abortError(tr("Unable to load anime name from webpage!"));
			}
		} else if(reply->error() != QNetworkReply::OperationCanceledError)
			this->abortError(reply->errorString());
	}

	reply->deleteLater();
}

void AddAnimeDialog::tryCompleted()
{
	if(this->activeReplies.isEmpty()) {
		this->ui->proxerIDLineEdit->setEnabled(true);
		this->ui->titleLineEdit->setText(this->cTitle);
		this->ui->previewLabel->setScaledContents(true);
		this->ui->previewLabel->setPixmap(this->cPixmap);
		this->loadingMovie->stop();
		this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}

void AddAnimeDialog::abortError(const QString &error)
{
	foreach (auto reply, this->activeReplies)
		reply->abort();
	this->activeReplies.clear();
	this->ui->proxerIDLineEdit->setEnabled(true);
	this->ui->titleLineEdit->clear();
	this->ui->previewLabel->setScaledContents(false);
	this->ui->previewLabel->setText(QStringLiteral("<i>preview</i>"));
	this->loadingMovie->stop();

	DialogMaster::critical(this, error, tr("Network Error"));
}
