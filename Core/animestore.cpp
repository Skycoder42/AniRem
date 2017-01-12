#include "animestore.h"
#include <QtConcurrent>
#include <QDir>
#include <QStandardPaths>
#include <QtSql>

#define EXEC_QUERY(query) do {\
	if(!query.exec()) {\
		emit storeError(query.lastError().text());\
		return;\
	}\
} while(false)

const QString AnimeStore::dbName("Animes_db");

AnimeStore::AnimeStore(QObject *parent) :
	QObject(parent),
	tp(new QThreadPool(this)),
	lock(),
	infoMap()
{
	tp->setMaxThreadCount(1);//1 thread to simply make it async

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
	QSqlDatabase::database(dbName).close();
	QSqlDatabase::removeDatabase(dbName);
}

AnimeList AnimeStore::animeInfoList() const
{
	return infoMap.values();
}

void AnimeStore::saveAnime(AnimePtr info)
{
	auto update = false;
	if(infoMap.contains(info->id())) {
		update = true;
		Q_ASSERT(infoMap.value(info->id()) == info);
	} else {
		infoMap.insert(info->id(), info);
		emit animeInfoListChanged(infoMap.values());
	}

	lock.addLock();
	QtConcurrent::run(tp, [this, info, update]() {
		CountLocker locker(lock, true);

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery infoQuery(db);
		if(update)
			infoQuery.prepare("UPDATE Animes SET Title = :title, Seasons = :seasons, Changed = :changed WHERE Id = :id");
		else
			infoQuery.prepare("INSERT INTO Animes (Id, Title, Seasons, Changed) VALUES(:id, :title, :seasons, :changed)");
		infoQuery.bindValue(":id", info->id());
		infoQuery.bindValue(":title", info->title());
		infoQuery.bindValue(":seasons", info->lastKnownSeasons());
		infoQuery.bindValue(":changed", info->hasNewSeasons());
		EXEC_QUERY(infoQuery);
	});
}

void AnimeStore::saveAll(AnimeList infoList)
{
	setInternal(infoList, false);

	lock.addLock();
	QtConcurrent::run(tp, [this, infoList]() {
		CountLocker locker(lock, true);

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
			infoQuery.bindValue("id", info->id());
			infoQuery.bindValue("title", info->title());
			infoQuery.bindValue("seasons", info->lastKnownSeasons());
			infoQuery.bindValue("changed", info->hasNewSeasons());
			EXEC_QUERY(infoQuery);
		}

		db.close();
	});
}

void AnimeStore::forgetAnime(int id)
{
	infoMap.remove(id);

	lock.addLock();
	QtConcurrent::run(tp, [this, id](){
		CountLocker locker(lock, true);

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
}

void AnimeStore::loadAnimes()
{
	lock.addLock();
	QtConcurrent::run(tp, [this](){
		CountLocker locker(lock, true);

		AnimeList infoList;

		auto db = QSqlDatabase::database(dbName);
		if(!db.isOpen()) {
			emit storeError(db.lastError().text());
			return;
		}

		QSqlQuery loadQuery(db);
		loadQuery.prepare("SELECT Id, Title, Seasons, Changed FROM Animes");
		EXEC_QUERY(loadQuery);

		while (loadQuery.next()) {
			auto info = AnimePtr::create(loadQuery.value(0).toInt(),
										 loadQuery.value(1).toString());
			info->setLastKnownSeasons(loadQuery.value(2).toInt());
			info->setHasNewSeasons(loadQuery.value(3).toBool());
			infoList.append(info);
		}

		QMetaObject::invokeMethod(this, "setInternal", Qt::QueuedConnection,
								  Q_ARG(AnimeList, infoList),
								  Q_ARG(bool, true));
	});
}

void AnimeStore::setInternal(AnimeList infoList, bool emitComplete)
{
	infoMap.clear();
	foreach(auto ptr, infoList)
		infoMap.insert(ptr->id(), ptr);
	emit animeInfoListChanged(infoMap.values());
	if(emitComplete)
		emit loadingCompleted();
}

void AnimeStore::saveQuitApp()
{
	lock.fullLock();//waits until all locks are released
}
