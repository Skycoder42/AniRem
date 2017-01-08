#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QNetworkAccessManager>
#include <QObject>

#include <functional>

class ImageLoader : public QObject//TODO async
{
	Q_OBJECT
public:
	explicit ImageLoader(QObject *parent = nullptr);

public slots:
	void loadImage(int id, std::function<void(int, QPixmap)> imgFunc);

signals:
	void imageLoaded(int id, const QPixmap &pixmap);
	void loadError(const QString &error);

private:
	static const char *IdProperty;

	QNetworkAccessManager *nam;
	QHash<int, QPixmap> cache;

	QPixmap getPixmap(int id, QNetworkReply *reply);
};

#endif // IMAGELOADER_H
