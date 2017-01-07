#include "animestore.h"
#include <QtConcurrent>
#include <QDir>
#include <QStandardPaths>
#include <QtSql>

#define R_LOCK() QReadLocker lock(&this->saveLock)
#define W_LOCK() QWriteLocker lock(&this->saveLock)

#define EXEC_QUERY(query) do {\
	if(!query.exec()) {\
		emit storeError(query.lastError().text());\
		db.close();\
		return;\
	}\
} while(false)

const QString AnimeStore::dbName("Animes_db");

AnimeStore::AnimeStore(QObject *parent) :
	QObject(parent),
	tp(new QThreadPool(this)),
	saveLock(),
	canSave(true),
	infoList()
{
	this->tp->setMaxThreadCount(1);//1 thread to keep the order

	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	dir.mkpath(".");
	auto path = dir.absoluteFilePath("animestore.db");
	if(QFile::copy(":/template.db", path))
		QFile::setPermissions(path, QFile::ReadUser | QFile::WriteUser);
	auto db = QSqlDatabase::addDatabase("QSQLITE", dbName);
	db.setDatabaseName(path);

	connect(qApp, &QCoreApplication::aboutToQuit,
			this, &AnimeStore::saveQuitApp,
			Qt::DirectConnection);

	QMetaObject::invokeMethod(this, "loadAnimes", Qt::QueuedConnection);
}

AnimeStore::~AnimeStore()
{
	QSqlDatabase::removeDatabase(dbName);
}

QList<AnimeInfo> AnimeStore::animeInfoList() const
{
	return this->infoList;
}

void AnimeStore::saveAnime(AnimeInfo info)
{
	auto index = -1;
	for(auto i = 0; i < this->infoList.size(); i++) {
		if(this->infoList[i].id() == info.id()) {
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

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery infoQuery(db);
		if(index == -1)
			infoQuery.prepare("INSERT INTO Animes (Id, Title, Seasons, Changed) VALUES(:id, :title, :seasons, :changed)");
		else
			infoQuery.prepare("UPDATE Animes SET Title = :title, Season = :season, Changed = :changed WHERE Id = :id");
		infoQuery.bindValue(":id", info.id());
		infoQuery.bindValue(":title", info.title());
		infoQuery.bindValue(":seasons", info.lastKnownSeasons());
		infoQuery.bindValue(":changed", info.hasNewSeasons());
		EXEC_QUERY(infoQuery);

		db.close();
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

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery truncQuery(db);
		truncQuery.prepare("DELETE FROM Animes");
		EXEC_QUERY(truncQuery);

		foreach(auto info, infoList) {
			QSqlQuery infoQuery(db);
			infoQuery.prepare("INSERT INTO Animes (Id, Title, Seasons, Changed) VALUES(:id, :title, :seasons, :changed)");
			infoQuery.bindValue("id", info.id());
			infoQuery.bindValue("title", info.title());
			infoQuery.bindValue("seasons", info.lastKnownSeasons());
			infoQuery.bindValue("changed", info.hasNewSeasons());
			EXEC_QUERY(infoQuery);
		}

		db.close();
	});
}

bool AnimeStore::forgetAnime(int id)
{
	auto didRemove = false;
	for(auto it = this->infoList.begin(); it != this->infoList.end(); it++) {
		if(it->id() == id) {
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

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery deleteQuery(db);
		deleteQuery.prepare("DELETE FROM Animes WHERE Id = ?");
		deleteQuery.addBindValue(id);
		EXEC_QUERY(deleteQuery);

		db.close();
	});

	return true;
}

void AnimeStore::loadAnimes()
{
	QtConcurrent::run(this->tp, [this](){
		R_LOCK();
		QList<AnimeInfo> infoList;

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery loadQuery(db);
		loadQuery.prepare("SELECT Id, Title, Seasons, Changed FROM Animes");
		EXEC_QUERY(loadQuery);

		while (loadQuery.next()) {
			AnimeInfo info(loadQuery.value(0).toInt(),
						   loadQuery.value(1).toString());
			info.setLastKnownSeasons(loadQuery.value(2).toInt());
			info.setHasNewSeasons(loadQuery.value(3).toBool());
			infoList.append(info);
		}

		QMetaObject::invokeMethod(this, "setInternal", Qt::QueuedConnection,
								  Q_ARG(QList<AnimeInfo>, infoList));
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
