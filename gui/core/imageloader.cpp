#include "imageloader.h"
#include <QDir>
#include <QImageReader>
#include <QStandardPaths>
#include <QUuid>
#include <QtConcurrent>

const QString ImageLoader::CacheDirName = QStringLiteral("preview-images");
const QString ImageLoader::ImageNameTemplate = QStringLiteral("img_%1.png");

ImageLoader::ImageLoader(QObject *parent) :
	QObject(parent),
	_nam(nullptr),
	_cache()
{}

void ImageLoader::loadImage(int id)
{
	QMetaObject::invokeMethod(this, "loadImageImpl", Qt::QueuedConnection,
							  Q_ARG(int, id));
}

void ImageLoader::clearCache()
{
	QMetaObject::invokeMethod(this, "clearCacheImpl", Qt::QueuedConnection);
}

void ImageLoader::loadImageImpl(int id)
{
	// check if cached
	auto imagePtr = _cache.object(id);
	if(imagePtr) {
		emit imageLoaded(id, *imagePtr);
		return;
	}

	// check if in cache dir
	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	if(cacheDir.mkpath(CacheDirName) && cacheDir.cd(CacheDirName)) {
		auto imgPath = cacheDir.absoluteFilePath(ImageNameTemplate.arg(id));
		if(QFile::exists(imgPath)) {
			QImage image(imgPath, "png");
			if(!image.isNull()) {
				_cache.insert(id, new QImage(image));
				emit imageLoaded(id, image);
				return;
			}
		}
	} else
		qWarning() << "Cache directory is not accessible";


	if(!_nam)
		_nam = new QNetworkAccessManager(this);
	auto reply = _nam->get(QNetworkRequest(QStringLiteral("https://cdn.proxer.me/cover/%1.jpg").arg(id)));
	connect(reply, &QNetworkReply::finished, this, [=](){
		imageNetworkReply(id, reply);
	}, Qt::QueuedConnection);
}

void ImageLoader::clearCacheImpl()
{
	_cache.clear();

	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	if(cacheDir.exists(CacheDirName)) {
		QString rmName = QStringLiteral(".rm-dir-") + QUuid::createUuid().toString();
		if(cacheDir.rename(CacheDirName, rmName)) {
			if(cacheDir.cd(rmName)) {
				QtConcurrent::run([cacheDir](){
					auto cDir = cacheDir;
					cDir.removeRecursively();
				});
			}
		} else if(cacheDir.cd(CacheDirName))
			cacheDir.removeRecursively();
	}
}

void ImageLoader::imageNetworkReply(int id, QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError) {
		QImageReader reader(reply, "jpg");
		QImage image;
		if(reader.read(&image)) {
			// send result
			_cache.insert(id, new QImage(image));
			emit imageLoaded(id, image);

			//save to cache
			QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
			if(cacheDir.mkpath(CacheDirName) && cacheDir.cd(CacheDirName)) {
				auto imgPath = cacheDir.absoluteFilePath(ImageNameTemplate.arg(id));
				if(!image.save(imgPath, "png"))
					qWarning() << "Failed to store image with id" << id << "to cache";
			} else
				qWarning() << "Cache directory is not accessible";
		} else
			emit imageLoadFailed(id, QStringLiteral("Failed to read image with id %1 with error: %2").arg(id).arg(reader.errorString()));
	} else
		emit imageLoadFailed(id, QStringLiteral("Network Error for id %1: %2").arg(id).arg(reply->errorString()));

	reply->deleteLater();
}
