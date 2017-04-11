#ifndef CACHINGNAM_H
#define CACHINGNAM_H

#include <QObject>
#include <QNetworkAccessManager>

class CachingNam : public QNetworkAccessManager
{
	Q_OBJECT

public:
	explicit CachingNam(QObject *parent = nullptr);

protected:
	QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) override;
};

#endif // CACHINGNAM_H
