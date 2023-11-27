#include "DayWorkPage.hpp"

#include "AddTimeDialog.hpp"
#include "DatabaseManager.hpp"
#include "MainWindow.hpp"
#include "QDateTime"
#include "SoundPlayer.hpp"
#include "StopWatch.hpp"
#include "Utility.hpp"

#include <QMessageBox>

namespace
{
QString timeUnitToString(const Timer::TimeUnit& timeUnit)
{
	QString hrs = QString::number(timeUnit.hrs);
	QString mnts = QString::number(timeUnit.mnts);
	QString secs = QString::number(timeUnit.secs);

	if (hrs.size() == 1)
	{
		hrs = '0' + hrs;
	}
	if (mnts.size() == 1)
	{
		mnts = '0' + mnts;
	}
	if (secs.size() == 1)
	{
		secs = '0' + secs;
	}

	return hrs + ':' + mnts + ':' + secs;
}
}	 // namespace

DayWorkPage::DayWorkPage(Page::Context context, QWidget* parent)
	: Page{context, parent}
	, mTimer{nullptr}
	, mMainLayout{nullptr}
	, mPauseButton{nullptr}
	, mContinueButton{nullptr}
	, mFinishEarlyButton{nullptr}
	, mFinishButton{nullptr}
{
	mDate = toDate(getProgDataEntry(Page::ProgDataType::Date));
	qDebug() << mDate.toString();

	int totalMinutesLeft =
		toInt(getProgDataEntry(Page::ProgDataType::TotalMinutesLeftAtStart));
	mTimer = new Timer{Timer::TimeUnit(totalMinutesLeft / 60, totalMinutesLeft % 60)};

	//	mTimer = new Timer{Timer::TimeUnit{0, 0, 2}};

	getContext().mainWindow->setWindowIcon(MainWindow::Icon::BMO_Bright);
	setupCoreWidgets();
	setupCoreWidgetsConnections();
}

void DayWorkPage::setupCoreWidgets()
{
	mMainLayout = new QVBoxLayout{this};
	QLabel* dateLabel = new QLabel{mDate.toString("dd.MM.yyyy"), this};
	dateLabel->setStyleSheet("font: 18pt;");
	dateLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	QHBoxLayout* headerLayout = new QHBoxLayout{this};
	QHBoxLayout* buttonsLayout = new QHBoxLayout{this};
	mContinueButton = new QPushButton{"continue", this};
	mPauseButton = new QPushButton{"pause", this};
	mFinishEarlyButton = new QPushButton{"finish early", this};
	mFinishButton = new QPushButton{"finish", this};
	mInfoButton = new QPushButton{"info", this};
	mAddExtraTimeButton = new QPushButton{"add overtime", this};

	int buttonHeight{30};
	mInfoButton->setMinimumSize(50, buttonHeight);
	mAddExtraTimeButton->setMinimumSize(100, buttonHeight);

	mContinueButton->setMinimumSize(130, buttonHeight);
	mPauseButton->setMinimumSize(130, buttonHeight);
	mFinishButton->setMinimumSize(130, buttonHeight);
	mFinishEarlyButton->setMinimumSize(130, buttonHeight);

	mFinishButton->setVisible(false);

	headerLayout->addWidget(mInfoButton);
	headerLayout->addStretch();
	headerLayout->addWidget(dateLabel);
	headerLayout->addStretch();
	headerLayout->addWidget(mAddExtraTimeButton);

	buttonsLayout->addWidget(mContinueButton);
	buttonsLayout->addWidget(mPauseButton);
	mMainLayout->addLayout(headerLayout);
	mMainLayout->addWidget(mTimer, Qt::AlignCenter);
	mMainLayout->addLayout(buttonsLayout);
	mMainLayout->addWidget(mFinishEarlyButton);
	mMainLayout->addWidget(mFinishButton);

	setLayout(mMainLayout);
}

void DayWorkPage::timeIsUp()
{
	getContext().mainWindow->setWindowIcon(MainWindow::Icon::BMO_Orange);
	SoundPlayer::playSound(SoundPlayer::TimeIsUp);
	mFinishButton->setVisible(true);

	StopWatch* stopwatch = new StopWatch;
	int		   indx = mMainLayout->indexOf(mTimer);
	if (indx == -1)
	{
		// todo: add logs
		indx = 1;
	}
	mMainLayout->insertWidget(indx, stopwatch);
}

void DayWorkPage::proceed()
{
	mTimer->proceed();
	writeLog("Continue");
	getContext().mainWindow->setWindowIcon(MainWindow::Icon::BMO_Bright);
}

void DayWorkPage::pause()
{
	mTimer->pause();
	writeLog("Pause");
	getContext().mainWindow->setWindowIcon(MainWindow::Icon::BMO_Dark);
}

void DayWorkPage::setupCoreWidgetsConnections()
{
	auto mainWindow = getContext().mainWindow;
	//    connect(mContinueButton, &QPushButton::pressed, mTimer, &Timer::proceed);
	connect(mAddExtraTimeButton, &QPushButton::pressed, this,
		[this]()
		{
			AddTimeDialog addTimeDialog{this};
			auto		  result = addTimeDialog.exec();
			if (result == QDialog::Accepted)
			{
				int currentGoalMinutes =
					toInt(getProgDataEntry(Page::ProgDataType::GoalTotalMinutes));
				int extraTotalMinutes = addTimeDialog.getTotalMinutes();

				int resultGoalTotalMinutes = currentGoalMinutes + extraTotalMinutes;
				int extra_hrs = extraTotalMinutes / 60;
				int extra_mnts = extraTotalMinutes % 60;

				updateProgDataEntry(
					Page::ProgDataType::GoalTotalMinutes, resultGoalTotalMinutes);
				mTimer->append_time(Timer::TimeUnit{extra_hrs, extra_mnts});
			}
		});
	connect(mContinueButton, &QPushButton::pressed, this, &DayWorkPage::proceed);
	connect(mPauseButton, &QPushButton::pressed, this, &DayWorkPage::pause);
	connect(mFinishEarlyButton, &QPushButton::pressed, mainWindow,
		[this]()
		{
			addProgDataEntry(
				Page::ProgDataType::WorkDone, timeUnitToString(mTimer->getElapsedTime()));
			getContext().mainWindow->selectPage(MainWindow::Pages::End);
		});
	connect(mFinishButton, &QPushButton::pressed, mainWindow,
		[this]()
		{
			addProgDataEntry(
				Page::ProgDataType::WorkDone, timeUnitToString(mTimer->getElapsedTime()));
			getContext().mainWindow->selectPage(MainWindow::Pages::End);
		});

	connect(mInfoButton, &QPushButton::clicked, this,
		[this]()
		{
			int goalTotalMinutes =
				toInt(getProgDataEntry((Page::ProgDataType::GoalTotalMinutes)));
			int hrs = goalTotalMinutes / 60;
			int mnts = goalTotalMinutes % 60;
			QMessageBox::information(this, tr("work_day"),
				tr("Goal: %1:%2")
					.arg(hrs)
					.arg(QString::number(mnts).rightJustified(2, '0')),
				QMessageBox::Ok);
		});

	connect(mTimer, &Timer::finished, this, &DayWorkPage::timeIsUp);
	connect(mTimer, &Timer::minutePassed, this,
		[this]()
		{
			//
			DatabaseManager::instance().addWorkDayLog(
				mDate, QDateTime::currentSecsSinceEpoch());
		});
}
