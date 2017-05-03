#ifndef PROXERIMAGEPROVIDER_H
#define PROXERIMAGEPROVIDER_H

#include <QQuickAsyncImageProvider>
#include <QSharedPointer>
#include <QThreadStorage>
#include <imageloader.h>

class ProxerImageResponse : public QQuickImageResponse
{
	Q_OBJECT

public:
	ProxerImageResponse(int id, const QSize &requestedSize);

	QQuickTextureFactory *textureFactory() const override;
	QString errorString() const override;

public slots:
	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &errorString);

private:
	int id;
	QSize requestedSize;
	QImage result;
	QString error;
};

class ProxerImageProvider : public QQuickAsyncImageProvider
{
public:
	ProxerImageProvider();

	ImageType imageType() const override;
	Flags flags() const override;

	QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
	QThreadStorage<QSharedPointer<ImageLoader>> store;
};

#endif // PROXERIMAGEPROVIDER_H
