#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QImage>
#include <QMutex>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QReadWriteLock>
#include <QThreadStorage>
#include <QMultiHash>
#include <QCache>
#include <functional>

class ImageLoader : public QObject
{
	Q_OBJECT

public:
	explicit ImageLoader(QObject *parent = nullptr);

public slots:
	void loadImage(int id);

signals:
	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &errorString);

private:
	static QReadWriteLock cacheLock;
	static QCache<int, QImage> cache;

	static QMutex requestMutex;
	static QMultiHash<int, QPointer<ImageLoader>> activeRequests;

	QNetworkAccessManager *nam;

	void imageNetworkReply(int id, QNetworkReply *reply);
	void completeLoading(int id, const QImage &image);
	void failLoading(int id, const QString &errorString);
};

#endif // IMAGELOADER_H
