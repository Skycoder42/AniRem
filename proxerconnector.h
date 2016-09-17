#ifndef PROXERCONNECTOR_H
#define PROXERCONNECTOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QPixmap>

class ProxerConnector : public QObject
{
	Q_OBJECT

public:
	explicit ProxerConnector(QObject *parent = nullptr);

public slots:
	void loadMetaData(int id);

signals:
	void metaDataLoaded(int id, QString title, QPixmap preview);
	void networkError(const QString &error);

private slots:
	void metaRequestFinished();

private:
	QNetworkAccessManager *nam;

	QString extractName(QNetworkReply *reply);
	QPixmap extractImage(QNetworkReply *reply);
};

#endif // PROXERCONNECTOR_H
