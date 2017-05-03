#include "proxerimageprovider.h"

ProxerImageProvider::ProxerImageProvider() :
	QQuickAsyncImageProvider(),
	store()
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
		if(!store.hasLocalData())
			store.setLocalData(QSharedPointer<ImageLoader>::create());
		auto loader = store.localData();
		auto response = new ProxerImageResponse(rId, requestedSize);
		QObject::connect(loader.data(), &ImageLoader::imageLoaded,
						 response, &ProxerImageResponse::imageLoaded);
		QObject::connect(loader.data(), &ImageLoader::imageLoadFailed,
						 response, &ProxerImageResponse::imageLoadFailed);
		loader->loadImage(rId);
		return response;
	} else
		return nullptr;
}



ProxerImageResponse::ProxerImageResponse(int id, const QSize &requestedSize) :
	id(id),
	requestedSize(requestedSize),
	result(),
	error()
{}

QQuickTextureFactory *ProxerImageResponse::textureFactory() const
{
	if(result.isNull())
		return nullptr;
	else
		return QQuickTextureFactory::textureFactoryForImage(result);
}

QString ProxerImageResponse::errorString() const
{
	return error;
}

void ProxerImageResponse::imageLoaded(int id, const QImage &image)
{
	if(id == this->id) {
		if(requestedSize.isValid())
			result = image.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		else
			result = image;
		emit finished();
	}
}

void ProxerImageResponse::imageLoadFailed(int id, const QString &errorString)
{
	if(id == this->id) {
		error = errorString;
		emit finished();
	}
}
