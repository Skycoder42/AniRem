#include "proxerimageprovider.h"
#include <QtMvvmCore/ServiceRegistry>

ProxerImageProvider::ProxerImageProvider() :
	QQuickAsyncImageProvider(),
	_loader(QtMvvm::ServiceRegistry::instance()->service<ImageLoader>())
{}

QQmlImageProviderBase::ImageType ProxerImageProvider::imageType() const
{
	return QQmlImageProviderBase::ImageResponse;
}

QQmlImageProviderBase::Flags ProxerImageProvider::flags() const
{
	return QQmlImageProviderBase::ForceAsynchronousImageLoading;
}

QQuickImageResponse *ProxerImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize)
{
	auto ok = false;
	auto rId = id.toInt(&ok);
	if(ok) {
		auto response = new ProxerImageResponse(rId, requestedSize);
		QObject::connect(_loader, &ImageLoader::imageLoaded,
						 response, &ProxerImageResponse::imageLoaded,
						 Qt::QueuedConnection);
		QObject::connect(_loader, &ImageLoader::imageLoadFailed,
						 response, &ProxerImageResponse::imageLoadFailed,
						 Qt::QueuedConnection);
		_loader->loadImage(rId);
		return response;
	} else
		return nullptr;
}



ProxerImageResponse::ProxerImageResponse(int id, const QSize &requestedSize) :
	_id(id),
	_requestedSize(requestedSize),
	_result(),
	_error()
{}

QQuickTextureFactory *ProxerImageResponse::textureFactory() const
{
	if(_result.isNull())
		return nullptr;
	else
		return QQuickTextureFactory::textureFactoryForImage(_result);
}

QString ProxerImageResponse::errorString() const
{
	return _error;
}

void ProxerImageResponse::cancel()
{
	_id = -1;
	_result = QImage();
	_error.clear();
	emit finished();
}

void ProxerImageResponse::imageLoaded(int id, const QImage &image)
{
	if(id == _id) {
		if(_requestedSize.isValid())
			_result = image.scaled(_requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		else
			_result = image;
		emit finished();
	}
}

void ProxerImageResponse::imageLoadFailed(int id, const QString &errorString)
{
	if(id == _id) {
		_error = errorString;
		emit finished();
	}
}
