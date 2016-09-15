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
	saveLock(),
	canSave(true)
{}

AnimeStore::~AnimeStore()
{
	W_LOCK();
	this->canSave = false;//TODO wait until ALL saves completed
}

void AnimeStore::loadAnimes()
{
	QtConcurrent::run([this](){
		R_LOCK();
		try {
			QList<AnimeInfo> infoList;
			foreach(auto value, this->loadList()) {
				auto obj = value.toObject();

				AnimeInfo info;
				info.id = obj[QStringLiteral("id")].toInt();
				info.title = obj[QStringLiteral("title")].toString();
				info.lastKnownSeasons = obj[QStringLiteral("seasons")].toInt();
				info.previewImage = QPixmap(this->imgPath(info.id));

				infoList.append(info);
			}

			emit loadingCompleted(infoList);
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

void AnimeStore::saveAnime(const AnimeInfo &info)
{
	QtConcurrent::run([this, info](){
		W_LOCK();
		if(!this->canSave)
			return;

		try {
			QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
			cacheDir.mkpath(QStringLiteral("./imgCache"));
			cacheDir.cd(QStringLiteral("./imgCache"));

			QJsonArray saveList = this->loadList();

			QJsonObject obj;
			obj[QStringLiteral("id")] = info.id;
			obj[QStringLiteral("title")] = info.title;
			obj[QStringLiteral("seasons")] = info.lastKnownSeasons;
			info.previewImage.save(this->imgPath(info.id));

			saveList.append(obj);
			this->saveList(saveList);
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

void AnimeStore::forgetAnime(int id)
{
	QtConcurrent::run([this, id](){
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
			else
				emit storeError(tr("No Anime with id %1 to be removed!").arg(id));
		} catch(QString error) {
			emit storeError(error);
		}
	});
}

QJsonArray AnimeStore::loadList()
{
	QDir appDataFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.json")));
	if(!animeStore.exists())
		return QJsonArray();
	else if(!animeStore.open(QIODevice::ReadOnly | QIODevice::Text))
		throw animeStore.errorString();
	else {
		QJsonParseError error;
		auto doc = QJsonDocument::fromJson(animeStore.readAll(), &error);
		animeStore.close();
		if(error.error != QJsonParseError::NoError)
			throw error.errorString();
		else
			return doc.array();
	}
}

void AnimeStore::saveList(const QJsonArray &array)
{
	QDir appDataFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	appDataFolder.mkpath(QStringLiteral("."));
	QSaveFile animeStore(appDataFolder.absoluteFilePath(QStringLiteral("animestore.json")));

	if(!animeStore.open(QIODevice::WriteOnly | QIODevice::Text))
		throw animeStore.errorString();
	else {
		QJsonDocument doc(array);
		animeStore.write(doc.toJson(QJsonDocument::Indented));
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
