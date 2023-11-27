#include "Timer.hpp"

#include <QTime>

Timer::Timer(TimeUnit timeUnit, QWidget* parent)
	: QLCDNumber(parent)
	, mStartTime(timeUnit)
	, mRemnTime(timeUnit)
	, mPause(false)
	, mFinished(false)
	, mSecondsCnt{0}
{
	setSegmentStyle(Filled);
	setDigitCount(8);
	mTimer = new QTimer(this);
	connect(mTimer, &QTimer::timeout, this, &Timer::update);

	setWindowTitle(tr("Digital Clock"));

	mTimer->start(1000);

	showTime();
}

void Timer::updateTime(int diffSec)
{
	mRemnTime.secs += diffSec;
	if (mRemnTime.secs < 0)
	{
		mRemnTime.secs = 59;
		mRemnTime.mnts--;
		if (mRemnTime.mnts < 0)
		{
			mRemnTime.mnts = 59;
			mRemnTime.hrs--;
			if (mRemnTime.hrs < 0)
			{
				emit finished();
				mFinished = true;
			}
		}
	}
}

Timer::TimeUnit Timer::getElapsedTime() const
{
	TimeUnit elapsedTime;
	elapsedTime.hrs = mStartTime.hrs - mRemnTime.hrs;
	elapsedTime.mnts = mStartTime.mnts - mRemnTime.mnts;
	elapsedTime.secs = mStartTime.secs - mRemnTime.secs;

	if (elapsedTime.secs < 0)
	{
		elapsedTime.mnts--;
		elapsedTime.secs += 60;
		if (elapsedTime.mnts < 0)
		{
			elapsedTime.hrs--;
			elapsedTime.mnts += 60;
		}
	}

	return elapsedTime;
}

#include <QDebug>
void Timer::append_time(const TimeUnit& extraTime)
{
	qDebug() << extraTime.hrs << " " << extraTime.mnts << " " << extraTime.secs;
	mStartTime += extraTime;
	mRemnTime += extraTime;
	update();
}

void Timer::proceed()
{
	if (!mPause)
		return;

	mTimer->start(1000);
	mPause = false;
}

void Timer::pause()
{
	if (mPause)
		return;

	mTimer->stop();
	mPause = true;
}

void Timer::update()
{
	if (mFinished)
		return;

	mSecondsCnt++;
	if (mSecondsCnt == 60)
	{
		mSecondsCnt = 0;
		emit minutePassed();
	}
	updateTime();
	showTime();
}

void Timer::showTime()
{
	if (!mFinished)
	{
		QTime time;
		time.setHMS(mRemnTime.hrs, mRemnTime.mnts, mRemnTime.secs);

		QString text = time.toString("hh:mm:ss");
		if ((time.second() % 2) == 0)
		{
			text[2] = ' ';
			text[5] = ' ';
		}
		display(text);
	}
	else
	{
		display(QString("00-00-00"));
	}
}
