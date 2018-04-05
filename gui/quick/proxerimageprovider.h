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
	ProxerImageResponse(int _id, const QSize &_requestedSize);

	QQuickTextureFactory *textureFactory() const override;
	QString errorString() const override;

public slots:
	void cancel() override;

	void imageLoaded(int _id, const QImage &image);
	void imageLoadFailed(int _id, const QString &errorString);

private:
	int _id;
	QSize _requestedSize;
	QImage _result;
	QString _error;
};

class ProxerImageProvider : public QQuickAsyncImageProvider
{
public:
	ProxerImageProvider();

	ImageType imageType() const override;
	Flags flags() const override;

	QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
	ImageLoader *_loader;
};

#endif // PROXERIMAGEPROVIDER_H
