#include "imageloader.h"
#include <core.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>

const char *ImageLoader::IdProperty = "__imgloader_request_id";

ImageLoader::ImageLoader(QObject *parent) :
	QObject(parent),
	nam(Core::createImageLoaderNam(this)),
	cache()
{}

void ImageLoader::loadImage(int id, std::function<void (int, QPixmap)> imgFunc)
{
	if(cache.contains(id))
		imgFunc(id, cache.value(id));
	else {
		auto reply = nam->get(QNetworkRequest(QStringLiteral("https://cdn.proxer.me/cover/%1.jpg").arg(id)));//TODO get is blocking
		connect(reply, &QNetworkReply::finished, this, [=](){
			auto pm = getPixmap(id, reply);
			if(!pm.isNull())
				imgFunc(id, pm);
			reply->deleteLater();
		}, Qt::QueuedConnection);
	}
}

QPixmap ImageLoader::getPixmap(int id, QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError) {
		QPixmap pm;
		if(pm.loadFromData(reply->readAll(), "jpg")) {
			cache.insert(id, pm);
			return pm;
		} else
			emit loadError(tr("Unable to load preview image. Data corrupted"));
	} else
		emit loadError(tr("Network Error: ") + reply->errorString());

	return {};
}
