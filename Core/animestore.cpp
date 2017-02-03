#include "animestore.h"
#include <QtConcurrent>
#include <QDir>
#include <QStandardPaths>
#include <QtSql>
#include "coremessage.h"

#define EXEC_QUERY(query) do {\
	if(!query.exec()) {\
		db.rollback(); \
		QMetaObject::invokeMethod(this, "showError", Qt::QueuedConnection, Q_ARG(QString, query.lastError().text())); \
		return;\
	}\
} while(false)

#define CHECK_DB_OPEN(db) do {\
	if(!db.isOpen() || !db.transaction()) { \
		QMetaObject::invokeMethod(this, "showError", Qt::QueuedConnection, Q_ARG(QString, db.lastError().text())); \
		return; \
	} \
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

AnimeInfo *AnimeStore::animeInfo(int id) const
{
	return infoMap.value(id);
}

bool AnimeStore::containsAnime(int id) const
{
	return infoMap.contains(id);
}

void AnimeStore::saveAnime(AnimeInfo *info)
{
	auto update = false;
	if(infoMap.contains(info->id())) {
		update = true;
		Q_ASSERT(infoMap.value(info->id()) == info);
	} else {
		info->setParent(this);//take own
		infoMap.insert(info->id(), info);
		emit animeInfoListChanged(infoMap.values());
	}

	lock.addLock();
	QtConcurrent::run(tp, [this, info, update]() {
		CountLocker locker(lock, true);

		auto db = QSqlDatabase::database(dbName);
		CHECK_DB_OPEN(db);

		QSqlQuery infoQuery(db);
		if(update)
			infoQuery.prepare("UPDATE Animes SET Title = :title, LastUpdate = :lastUpdate WHERE Id = :id");
		else
			infoQuery.prepare("INSERT INTO Animes (Id, Title, LastUpdate) VALUES(:id, :title, :lastUpdate)");
		infoQuery.bindValue(":id", info->id());
		infoQuery.bindValue(":title", info->title());
		infoQuery.bindValue(":lastUpdate", info->lastUpdateCheck());
		EXEC_QUERY(infoQuery);

		for(auto it = info->seasonState().constBegin(); it != info->seasonState().constEnd(); it++) {
			QSqlQuery countQuery(db);
			countQuery.prepare("INSERT OR REPLACE INTO Seasons(Anime, Type, Count, Changed) VALUES(?, ?, ?, ?)");
			countQuery.addBindValue(info->id());
			countQuery.addBindValue((int)it.key());
			countQuery.addBindValue(it.value().first);
			countQuery.addBindValue(it.value().second);
			EXEC_QUERY(countQuery);
		}

		db.commit();
	});
}

void AnimeStore::forgetAnime(int id)
{
	infoMap.take(id)->deleteLater();

	lock.addLock();
	QtConcurrent::run(tp, [this, id](){
		CountLocker locker(lock, true);

		auto db = QSqlDatabase::database(dbName);
		CHECK_DB_OPEN(db);

		QSqlQuery deleteQuery(db);
		deleteQuery.prepare("DELETE FROM Animes WHERE Id = ?");
		deleteQuery.addBindValue(id);
		EXEC_QUERY(deleteQuery);

		QSqlQuery deleteCountQuery(db);
		deleteCountQuery.prepare("DELETE FROM Seasons WHERE Anime = ?");
		deleteCountQuery.addBindValue(id);
		EXEC_QUERY(deleteCountQuery);

		db.commit();
	});
}

void AnimeStore::loadAnimes()
{
	lock.addLock();
	QtConcurrent::run(tp, [this](){
		CountLocker locker(lock, true);

		AnimeList infoList;

		auto db = QSqlDatabase::database(dbName);
		CHECK_DB_OPEN(db);

		QSqlQuery loadQuery(db);
		loadQuery.prepare("SELECT Id, Title, LastUpdate FROM Animes");
		EXEC_QUERY(loadQuery);

		while (loadQuery.next()) {
			auto id = loadQuery.value(0).toInt();

			QSqlQuery countQuery(db);
			countQuery.prepare("SELECT Type, Count, Changed FROM Seasons WHERE Anime = ?");
			countQuery.addBindValue(id);
			EXEC_QUERY(countQuery);

			auto info = new AnimeInfo(id, loadQuery.value(1).toString());// parenting done in "set internal"
			info->setLastUpdateCheck(loadQuery.value(2).toDate());
			while(countQuery.next()) {
				info->setSeasonInfo((AnimeInfo::SeasonType)countQuery.value(0).toInt(),
									{countQuery.value(1).toInt(), countQuery.value(2).toBool()});
			}

			info->moveToThread(thread());
			infoList.append(info);
		}

		db.commit();
		QMetaObject::invokeMethod(this, "setInternal", Qt::QueuedConnection,
								  Q_ARG(AnimeList, infoList),
								  Q_ARG(bool, true));
	});
}

void AnimeStore::setInternal(AnimeList infoList, bool emitComplete)
{
	auto oldPtrs = infoMap.values();
	infoMap.clear();
	foreach(auto ptr, infoList) {
		infoMap.insert(ptr->id(), ptr);
		ptr->setParent(this);//take own
		oldPtrs.removeOne(ptr);
	}
	foreach (auto ptr, oldPtrs)
		ptr->deleteLater();
	emit animeInfoListChanged(infoMap.values());
	if(emitComplete)
		emit storeLoaded();
}

void AnimeStore::saveQuitApp()
{
	lock.fullLock();//waits until all locks are released
}

void AnimeStore::showError(const QString &error)
{
	CoreMessage::critical(tr("Data Error"), error);
}
