#include "animestore.h"
#include <QtConcurrent>
#include <QFile>
#include <QSaveFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#define R_LOCK() QReadLocker lock(&this->saveLock)
#define W_LOCK() QWriteLocker lock(&this->saveLock)

AnimeStore::AnimeStore(QObject *parent) :
	QObject(parent),
	tp(new QThreadPool(this)),
	saveLock(),
	canSave(true),
	infoList()
{
	this->tp->setMaxThreadCount(1);//1 thread to keep the order

	connect(qApp, &QCoreApplication::aboutToQuit,
			this, &AnimeStore::saveQuitApp,
			Qt::DirectConnection);

	QMetaObject::invokeMethod(this, "loadAnimes", Qt::QueuedConnection);
}

QList<AnimeInfo> AnimeStore::animeInfoList() const
{
	return this->infoList;
}

void AnimeStore::saveAnime(AnimeInfo info)
{
	auto index = -1;
	for(auto i = 0; i < this->infoList.size(); i++) {
		if(this->infoList[i].id == info.id) {
			index = i;
			break;
		}
	}
	if(index == -1)
		this->infoList.append(info);
	else
		this->infoList.replace(index, info);
	emit animeInfoListChanged(this->infoList);

	QtConcurrent::run(this->tp, [this, info, index](){
		W_LOCK();
		if(!this->canSave)
			return;

		try {
			QJsonArray saveList = this->loadList();

			QJsonObject obj;
			obj[QStringLiteral("id")] = info.id;
			obj[QStringLiteral("title")] = info.title;
			obj[QStringLiteral("seasons")] = info.lastKnownSeasons;
			obj[QStringLiteral("hasNew")] = info.hasNewSeasons;
			info.previewImage.save(this->imgPath(info.id));

			if(index == -1)
				saveList.append(obj);
			else {
				//sanity check
				if(saveList[index].toObject()[QStringLiteral("id")].toInt() != info.id)
					throw tr("DATA CORRUPTED! Cannot save data anymore. Please restart the application!");
				saveList.replace(index, obj);
			}
			this->saveList(saveList);
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

void AnimeStore::saveAll(QList<AnimeInfo> infoList)
{
	this->infoList = infoList;
	emit animeInfoListChanged(this->infoList);

	QtConcurrent::run(this->tp, [this, infoList](){
		W_LOCK();
		if(!this->canSave)
			return;

		try {
			QJsonArray saveList;

			foreach(auto info, infoList) {
				QJsonObject obj;
				obj[QStringLiteral("id")] = info.id;
				obj[QStringLiteral("title")] = info.title;
				obj[QStringLiteral("seasons")] = info.lastKnownSeasons;
				obj[QStringLiteral("hasNew")] = info.hasNewSeasons;
				info.previewImage.save(this->imgPath(info.id));
				saveList.append(obj);
			}
			this->saveList(saveList);
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

bool AnimeStore::forgetAnime(int id)
{
	auto didRemove = false;
	for(auto it = this->infoList.begin(); it != this->infoList.end(); it++) {
		if(it->id == id) {
			this->infoList.erase(it);
			didRemove = true;
			break;
		}
	}
	if(!didRemove)
		return false;
	else
		emit animeInfoListChanged(this->infoList);

	QtConcurrent::run(this->tp, [this, id](){
		W_LOCK();
		if(!this->canSave)
			return;

		try {
			QJsonArray saveList = this->loadList();

			bool found = false;
			for(auto i = 0; i < saveList.size(); i++) {
				auto obj = saveList[i].toObject();
				if(obj[QStringLiteral("id")].toInt() == id) {
					found = true;
					saveList.removeAt(i);
					QFile::remove(this->imgPath(id));
					break;
				}
			}

			if(found)
				this->saveList(saveList);
		} catch(QString error) {
			emit storeError(error);
		}
	});

	return true;
}

void AnimeStore::loadAnimes()
{
	QtConcurrent::run(this->tp, [this](){
		R_LOCK();
		try {
			QList<AnimeInfo> infoList;
			foreach(auto value, this->loadList()) {
				auto obj = value.toObject();

				AnimeInfo info;
				info.id = obj[QStringLiteral("id")].toInt();
				info.title = obj[QStringLiteral("title")].toString();
				info.lastKnownSeasons = obj[QStringLiteral("seasons")].toInt();
				info.hasNewSeasons = obj[QStringLiteral("hasNew")].toBool();
				info.previewImage = QPixmap(this->imgPath(info.id));

				infoList.append(info);
			}

			QMetaObject::invokeMethod(this, "setInternal", Qt::QueuedConnection,
									  Q_ARG(QList<AnimeInfo>, infoList));
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

void AnimeStore::setInternal(QList<AnimeInfo> infoList)
{
	this->infoList = infoList;
	emit animeInfoListChanged(infoList);
	emit loadingCompleted();
}

void AnimeStore::saveQuitApp()
{
	W_LOCK();
	this->canSave = false;//TODO wait until ALL saves completed
}

QJsonArray AnimeStore::loadList()
{
	QDir appDataFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
#ifdef QT_NO_DEBUG
	QFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.dat")));
#else
	QFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.json")));
#endif
	if(!animeStore.exists())
		return QJsonArray();
	else if(!animeStore.open(QIODevice::ReadOnly | QIODevice::Text))
		throw animeStore.errorString();
	else {
#ifdef QT_NO_DEBUG
		auto doc = QJsonDocument::fromBinaryData(animeStore.readAll());
#else
		auto doc = QJsonDocument::fromJson(animeStore.readAll());
#endif
		animeStore.close();
		if(doc.isNull())
			throw tr("Failed to load anime data");
		else
			return doc.array();
	}
}

void AnimeStore::saveList(const QJsonArray &array)
{
	QDir appDataFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	appDataFolder.mkpath(QStringLiteral("."));
#ifdef QT_NO_DEBUG
	QSaveFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.dat")));
#else
	QSaveFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.json")));
#endif

	if(!animeStore.open(QIODevice::WriteOnly | QIODevice::Text))
		throw animeStore.errorString();
	else {
		QJsonDocument doc(array);
#ifdef QT_NO_DEBUG
		animeStore.write(doc.toBinaryData());
#else
		animeStore.write(doc.toJson(QJsonDocument::Indented));
#endif
		if(!animeStore.commit())
			throw animeStore.errorString();
	}
}

QString AnimeStore::imgPath(int id)
{
	QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	cacheDir.mkpath(QStringLiteral("./imgCache"));
	cacheDir.cd(QStringLiteral("./imgCache"));
	return cacheDir.absoluteFilePath(QStringLiteral("pimg_%1.png").arg(id));
}
