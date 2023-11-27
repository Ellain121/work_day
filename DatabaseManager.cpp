#include "DatabaseManager.hpp"

#include "Utility.hpp"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace
{
const QString TABLE_NAME_INFO = "days_info";
const QString TABLE_NAME_LOGS = "days_logs";
const QString createInfoTableQuery =
	QString{"CREATE TABLE IF NOT EXISTS " + TABLE_NAME_INFO +
			" (date DATE UNIQUE NOT NULL,"
			"date_goal INT NOT NULL,"
			"date_result INT,"
			"info TEXT"
			");"};
const QString createLogsTableQuery =
	QString{"CREATE TABLE IF NOT EXISTS " + TABLE_NAME_LOGS +
			" (date DATE NOT NULL, timestamp TIMESTAMP NOT NULL);"};
}	 // namespace

void DatabaseManager::debugQuery(const QSqlQuery& query)
{
	if (query.lastError().type() == QSqlError::NoError)
	{
		qDebug() << "Query OK: " << query.lastQuery()
				 << "| Affected rows: " << query.numRowsAffected();
	}
	else
	{
		qDebug() << "===============================";
		qWarning() << "Query KO:" << query.lastError().text();
		qWarning() << "Query text:" << query.lastQuery();
		qDebug() << "===============================";
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager singletonDB{};
	return singletonDB;
}

DatabaseManager::DatabaseManager(const QString& path)
	: mDatabase{std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"))}
{
	mDatabase->setDatabaseName(path);

	bool openStatus = mDatabase->open();
	assert((openStatus == true) && "mDatabase->open failed!");

	if (!mDatabase->tables().contains(TABLE_NAME_INFO))
	{
		// table not exist, create one
		QSqlQuery query{*mDatabase};
		query.exec(createInfoTableQuery);
		DatabaseManager::debugQuery(query);
	}
	if (!mDatabase->tables().contains(TABLE_NAME_LOGS))
	{
		// table not exist, create one
		QSqlQuery query{*mDatabase};
		query.exec(createLogsTableQuery);
		DatabaseManager::debugQuery(query);
	}
}

bool DatabaseManager::isDateExist(const QDate& date)
{
	QSqlQuery query{*mDatabase};
	query.prepare("SELECT COUNT(*) cnt FROM days_info WHERE date=(:date)");
	query.bindValue(":date", date);
	query.exec();
	query.first();
	DatabaseManager::debugQuery(query);

	int cnt = toInt(query.value("cnt"));
	return cnt > 0;
}

WorkDay DatabaseManager::getWorkDay(const QDate& date)
{
	/********* [ Part 1, get general info ] ********/
	assert((isDateExist(date)) && "required date does not exist!");

	WorkDay	  wDay{date};
	QSqlQuery query_1{*mDatabase};
	query_1.prepare("SELECT date_goal, date_result, info FROM " + TABLE_NAME_INFO +
					" WHERE date=(:date)");
	query_1.bindValue(":date", date);
	query_1.exec();
	query_1.first();
	DatabaseManager::debugQuery(query_1);

	wDay.setGoal(toInt(query_1.value("date_goal")));
	//	wDay.setResult(toInt(query_1.value("date_result")));
	wDay.setInfo(toQString(query_1.value("info")));

	/********* [ Part 2, get log info( ] ********/
	QSqlQuery query_2{*mDatabase};
	query_2.prepare(
		"SELECT COUNT(*) cnt FROM " + TABLE_NAME_LOGS + " WHERE date=(:date)");
	query_2.bindValue(":date", date);
	query_2.exec();
	query_2.first();
	DatabaseManager::debugQuery(query_2);

	int cnt = toInt(query_2.value("cnt"));
	wDay.setResult(cnt);

	return wDay;
}

DatabaseManager::~DatabaseManager()
{
	mDatabase->close();
}

void DatabaseManager::addWorkDay(const WorkDay& workDay)
{
	QSqlQuery query{*mDatabase};
	query.prepare("INSERT INTO " + TABLE_NAME_INFO +
				  " (date, date_goal) VALUES (:date, :date_goal)");
	query.bindValue(":date", workDay.getDate().toString(Qt::ISODate));
	query.bindValue(":date_goal", workDay.getGoal());
	query.exec();
	DatabaseManager::debugQuery(query);
}

void DatabaseManager::addWorkDayLog(const QDate& date, qint64 timestamp)
{
	QSqlQuery query{*mDatabase};
	query.prepare("INSERT INTO " + TABLE_NAME_LOGS +
				  " (date, timestamp) VALUES (:date, :timestamp)");
	query.bindValue(":date", date.toString(Qt::ISODate));
	query.bindValue(":timestamp", timestamp);
	query.exec();
	DatabaseManager::debugQuery(query);
}

void DatabaseManager::updateWorkDay(const WorkDay& workDay)
{
	QSqlQuery query{*mDatabase};
	query.prepare("UPDATE " + TABLE_NAME_INFO +
				  " SET date=(:date), date_goal=(:date_goal), "
				  "date_result=(:date_result), info=(:info)");
	query.bindValue(":date", workDay.getDate().toString(Qt::ISODate));
	query.bindValue(":date_goal", workDay.getGoal());
	query.bindValue(":date_result", workDay.getResult());
	query.bindValue(":info", workDay.getInfo());
}
