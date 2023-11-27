#pragma once

#include <QLCDNumber>
#include <QTimer>

class Timer : public QLCDNumber
{
	Q_OBJECT

public:
	struct TimeUnit
	{
		TimeUnit(int hrs = 0, int mnts = 0, int secs = 0)
			: hrs(hrs)
			, mnts(mnts)
			, secs(secs)
		{
		}

		void operator+=(const TimeUnit& extraTimeUnit)
		{
			hrs += extraTimeUnit.hrs;
			mnts += extraTimeUnit.mnts;
			secs += extraTimeUnit.secs;

			if (secs >= 60)
			{
				mnts += 1;
				secs -= 60;
			}
			if (mnts >= 60)
			{
				hrs += 1;
				mnts -= 60;
			}
		}

		int hrs;
		int mnts;
		int secs;
	};

public:
	Timer(TimeUnit timeUnit, QWidget* parent = nullptr);

	TimeUnit getElapsedTime() const;
	void	 append_time(const TimeUnit& extraTime);

private:
	void updateTime(int diffSec = -1);
	void showTime();

signals:
	void finished();
	void minutePassed();

public slots:
	void proceed();
	void pause();

private slots:
	void update();

private:
	TimeUnit mStartTime;
	TimeUnit mRemnTime;
	QTimer*	 mTimer;

	bool mStarted;
	bool mPause;
	bool mFinished;

	int mSecondsCnt;
};
