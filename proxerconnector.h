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
	void loadSeasonCount(int id);

signals:
	void metaDataLoaded(int id, QString title, QPixmap preview);
	void seasonsLoaded(int id, int seasonCount);
	void networkError(const QString &error);

private slots:
	void metaRequestFinished();
	void seasonRequestFinished();

private:
	QNetworkAccessManager *nam;

	QString extractName(QNetworkReply *reply);
	QPixmap extractImage(QNetworkReply *reply);

	int findIndex(QString raw, QString target, int offset = 0, bool allowsError = false) const;
	void checkHentai(QString raw) const;
};

#endif // PROXERCONNECTOR_H
