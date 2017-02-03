#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QPixmap>

#include <functional>

class ImageLoader : public QObject
{
	Q_OBJECT

public:
	static ImageLoader *instance();

public slots:
	void loadImage(int id, std::function<void(int, QPixmap)> imgFunc);

signals:
	void imageLoaded(int id, const QPixmap &pixmap);
	void loadError(const QString &error);

private:
	static const char *IdProperty;
	static ImageLoader *_instance;

	QNetworkAccessManager *nam;
	QHash<int, QPixmap> cache;

	explicit ImageLoader(QObject *parent = nullptr);
	QPixmap getPixmap(int id, QNetworkReply *reply);
};

#endif // IMAGELOADER_H