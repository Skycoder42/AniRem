#include "imageloader.h"
#include <QDir>
#include <QImageReader>
#include <QStandardPaths>

QMutex ImageLoader::requestMutex;
QMultiHash<int, QPointer<ImageLoader>> ImageLoader::activeRequests;

ImageLoader::ImageLoader(QObject *parent) :
	QObject(parent),
	nam(nullptr)
{}

void ImageLoader::loadImage(int id)
{
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
		auto dir = QStringLiteral("anmimg");
		if(cacheDir.mkpath(dir) && cacheDir.cd(dir)) {
			auto imgPath = cacheDir.absoluteFilePath(QStringLiteral("img_%1.png").arg(id));
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
				reply->deleteLater();
			}, Qt::QueuedConnection);
		} else
			failLoading(id, tr("Cache directory is not accessible"));
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
			auto dir = QStringLiteral("anmimg");
			if(cacheDir.mkpath(dir) && cacheDir.cd(dir)) {
				auto imgPath = cacheDir.absoluteFilePath(QStringLiteral("img_%1.png").arg(id));
				if(!img.save(imgPath, "png"))
					qWarning() << "Failed to cache image with id" << id;
				completeLoading(id, img);
			} else
				failLoading(id, tr("Cache directory is not accessible"));
		} else
			failLoading(id, tr("Failed to read image with error: ") + reader.errorString());
	} else
		failLoading(id, tr("Network Error: ") + reply->errorString());
}

void ImageLoader::completeLoading(int id, const QImage &image)
{
	//notify all loaders
	QList<QPointer<ImageLoader>> loaders;
	{
		QMutexLocker _(&requestMutex);
		loaders = activeRequests.values(id);
		activeRequests.remove(id);
	}

	foreach(auto loader, loaders) {//includes myself
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

	foreach(auto loader, loaders) {//includes myself
		QMetaObject::invokeMethod(loader, "imageLoadFailed", Qt::QueuedConnection,
								  Q_ARG(int, id),
								  Q_ARG(QString, errorString));
	}
}
