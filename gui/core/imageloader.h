#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QImage>
#include <QNetworkReply>
#include <QObject>
#include <QCache>

class ImageLoader : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit ImageLoader(QObject *parent = nullptr);

public slots:
	void loadImage(int id);

signals:
	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &errorString);

private slots:
	void loadImageImpl(int id);

private:
	static const QString CacheDirName;
	static const QString ImageNameTemplate;

	QNetworkAccessManager *_nam;
	QCache<int, QImage> _cache;

	void imageNetworkReply(int id, QNetworkReply *reply);
};

#endif // IMAGELOADER_H
