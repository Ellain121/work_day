#include "DigitalClock.hpp"

#include <QTimer>
#include <QTime>
#include <QString>

DigitalClock::DigitalClock(QWidget* parent)
: QLCDNumber(parent)
{
    setSegmentStyle(Filled);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DigitalClock::showTime);
    timer->start(1000);

    showTime();

    setWindowTitle(tr("Digital Clock"));
    resize(150, 150);
}

void DigitalClock::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("HH:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    display(text);
}