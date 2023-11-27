#pragma once

#include <QDate>
#include <QString>

class WorkDay
{
public:
	WorkDay(const QDate& date = QDate::currentDate(), int goal = 0, int result = 0,
		const QString& info = "");

	void setDate(const QDate& date);
	void setGoal(int goal);
	void setResult(int result);
	void setInfo(const QString& info);

	const QDate&   getDate() const;
	int			   getGoal() const;
	int			   getResult() const;
	const QString& getInfo() const;

private:
	QDate	mDate;
	int		mGoal;
	int		mResult;
	QString mInfo;
};
