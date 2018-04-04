#include "imageloader.h"
#include <QDir>
#include <QImageReader>
#include <QStandardPaths>

const QString ImageLoader::CacheDirName = QStringLiteral("preview-images");
const QString ImageLoader::ImageNameTemplate = QStringLiteral("img_%1.png");

QReadWriteLock ImageLoader::cacheLock;
QCache<int, QImage> ImageLoader::cache;

QMutex ImageLoader::requestMutex;
QMultiHash<int, QPointer<ImageLoader>> ImageLoader::activeRequests;

ImageLoader::ImageLoader(QObject *parent) :
	QObject(parent),
	nam(nullptr)
{}

void ImageLoader::loadImage(int id)
{
	//try to load image from cache
	{
		QReadLocker _(&cacheLock);
		auto image = cache.object(id);
		if(image) {
			emit imageLoaded(id, *image);
			return;
		}
	}

	//load the image or register for info
	auto isMe = false;
	{
		QMutexLocker _(&requestMutex);
		if(!activeRequests.contains(id))
			isMe = true;
		activeRequests.insert(id, this);
	}

	if(isMe) {
		QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
		if(cacheDir.mkpath(CacheDirName) && cacheDir.cd(CacheDirName)) {
			auto imgPath = cacheDir.absoluteFilePath(ImageNameTemplate.arg(id));
			if(QFile::exists(imgPath)) {
				QImage image(imgPath, "png");
				if(!image.isNull()) {
					completeLoading(id, image);
					return;
				}
			}

			if(!nam)
				nam = new QNetworkAccessManager(this);
			auto reply = nam->get(QNetworkRequest(QStringLiteral("https://cdn.proxer.me/cover/%1.jpg").arg(id)));
			connect(reply, &QNetworkReply::finished, this, [=](){
				imageNetworkReply(id, reply);
			}, Qt::QueuedConnection);
		} else
			failLoading(id, QStringLiteral("Cache directory is not accessible"));
	}
}

void ImageLoader::imageNetworkReply(int id, QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError) {
		QImageReader reader(reply, "jpg");
		QImage img;
		if(reader.canRead() && reader.read(&img)) {
			//save to file first
			QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
			if(cacheDir.mkpath(CacheDirName) && cacheDir.cd(CacheDirName)) {
				auto imgPath = cacheDir.absoluteFilePath(ImageNameTemplate.arg(id));
				if(!img.save(imgPath, "png"))
					qWarning() << "Failed to store image with id" << id << "to cache";
				completeLoading(id, img);
			} else
				failLoading(id, QStringLiteral("Cache directory is not accessible"));
		} else
			failLoading(id, QStringLiteral("Failed to read image with id %1 with error: %2").arg(id).arg(reader.errorString()));
	} else
		failLoading(id, QStringLiteral("Network Error for id %1: %2").arg(id).arg(reply->errorString()));

	reply->deleteLater();
}

void ImageLoader::completeLoading(int id, const QImage &image)
{
	//cache the image
	{
		QWriteLocker _(&cacheLock);
		cache.insert(id, new QImage(image));
	}
	//notify all loaders
	QList<QPointer<ImageLoader>> loaders;
	{
		QMutexLocker _(&requestMutex);
		loaders = activeRequests.values(id);
		activeRequests.remove(id);
	}

	for(const auto &loader : loaders) {//includes myself
		QMetaObject::invokeMethod(loader, "imageLoaded", Qt::QueuedConnection,
								  Q_ARG(int, id),
								  Q_ARG(QImage, image));
	}
}

void ImageLoader::failLoading(int id, const QString &errorString)
{
	//notify all loaders
	QList<QPointer<ImageLoader>> loaders;
	{
		QMutexLocker _(&requestMutex);
		loaders = activeRequests.values(id);
		activeRequests.remove(id);
	}

	for(const auto &loader : loaders) {//includes myself
		QMetaObject::invokeMethod(loader, "imageLoadFailed", Qt::QueuedConnection,
								  Q_ARG(int, id),
								  Q_ARG(QString, errorString));
	}
}
