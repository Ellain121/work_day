#include "WorkDay.hpp"

WorkDay::WorkDay(const QDate& date, int goal, int result, const QString& info)
	: mDate{date}
	, mGoal{goal}
	, mResult{result}
	, mInfo{info}
{
}

void WorkDay::setDate(const QDate& date)
{
	mDate = date;
}

void WorkDay::setGoal(int goal)
{
	mGoal = goal;
}

void WorkDay::setResult(int result)
{
	mResult = result;
}

void WorkDay::setInfo(const QString& info)
{
	mInfo = info;
}

const QDate& WorkDay::getDate() const
{
	return mDate;
}

int WorkDay::getGoal() const
{
	return mGoal;
}

int WorkDay::getResult() const
{
	return mResult;
}

const QString& WorkDay::getInfo() const
{
	return mInfo;
}
