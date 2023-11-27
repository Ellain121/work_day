#pragma once

#include <QLCDNumber>
#include <QTime>

class StopWatch : public QLCDNumber
{
	Q_OBJECT

public:
	StopWatch(QWidget* parent = nullptr);

	QTime getElapsedTime() const;

private:
	void updateTime(int diffSec = -1);
	void showTime();

private slots:
	void update();

private:
	QTime	mTime;
	QTimer* mTimer;
};
