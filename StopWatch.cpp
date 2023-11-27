#include "StopWatch.hpp"

#include <QTimer>

StopWatch::StopWatch(QWidget* parent)
	: QLCDNumber{parent}
	, mTime{0, 0, 0}
	, mTimer{new QTimer{this}}
{
	/**** QLCDNumber settings ****/
	setSegmentStyle(Flat);
	setDigitCount(8);
	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor{255, 150, 85});
	setPalette(pal);
	/********************************/

	connect(mTimer, &QTimer::timeout, this, &StopWatch::update);
	mTimer->start(1000);

	showTime();
}

void StopWatch::showTime()
{
	QString timeTxt = mTime.toString("hh:mm:ss");
	if ((mTime.second() % 2) == 0)
	{
		timeTxt[2] = ' ';
		timeTxt[5] = ' ';
	}

	display(timeTxt);
}

void StopWatch::update()
{
	mTime = mTime.addSecs(1);
	showTime();
}
