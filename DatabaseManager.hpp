#pragma once

#include "NonCopyable.hpp"
#include "WorkDay.hpp"

#include <QString>
#include <memory>

class QSqlDatabase;
class QSqlQuery;

const QString DATABASE_FILENAME = "main_db.sqlite";

class DatabaseManager : private NonCopyable
{
public:
	static DatabaseManager& instance();
	~DatabaseManager();

	static void debugQuery(const QSqlQuery& query);

private:
	DatabaseManager(const QString& path = DATABASE_FILENAME);
	DatabaseManager& operator=(const DatabaseManager& rhs) = delete;

public:
	bool	isDateExist(const QDate& date);
	WorkDay getWorkDay(const QDate& date);

	void addWorkDay(const WorkDay& workDay);
	void addWorkDayLog(const QDate& date, qint64 timestamp);
	void updateWorkDay(const WorkDay& workDay);

private:
	std::unique_ptr<QSqlDatabase> mDatabase;
};
