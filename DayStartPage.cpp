#include "DayStartPage.hpp"

#include "AddTimeDialog.hpp"
#include "DatabaseManager.hpp"
#include "MainWindow.hpp"
#include "Utility.hpp"

#include <QDebug>
#include <QMessageBox>

namespace
{

const int startHourNumber = 0;
const int endHourNumber = 10;

int hoursIndxToHours(int indx)
{
	return indx + startHourNumber;
}

int minutesIndxToMinuts(int indx)
{
	return indx * 15;
}

int hoursToHoursIndx(int hrs)
{
	assert((hrs >= startHourNumber && hrs <= endHourNumber) && "Invalid hrs number!");
	return hrs -= startHourNumber;
}

int minutesToMinutesIndx(int mnts)
{
	assert((mnts % 15 == 0 && mnts <= 45) && "Invalid hrs number!");
	return mnts / 15;
}

}	 // namespace

DayStartPage::DayStartPage(Page::Context context, QWidget* parent)
	: Page{context, parent}
	, mMainLayout{nullptr}
	, mHoursComboBox{nullptr}
	, mMinutesComboBox{nullptr}
	, mStartPushButton{nullptr}
	, mAddTimePushButton{nullptr}
	, mChooseDateCalendar{nullptr}
	, mIsDayAlreadyStarted{false}
{
	setupCoreWidgets();
	setupCoreWidgetsConnections();
}

void DayStartPage::setupCoreWidgets()
{
	mMainLayout = new QVBoxLayout{this};
	mChooseDateCalendar = new QCalendarWidget{this};
	mHoursComboBox = new QComboBox{this};
	mMinutesComboBox = new QComboBox{this};
	mStartPushButton = new QPushButton{"start", this};
	mAddTimePushButton = new QPushButton{"add time", this};

	QSizePolicy spRetain = mAddTimePushButton->sizePolicy();
	spRetain.setRetainSizeWhenHidden(true);
	mAddTimePushButton->setSizePolicy(spRetain);
	mAddTimePushButton->setVisible(false);

	QHBoxLayout* chooseLayout = new QHBoxLayout{this};

	for (int i = startHourNumber; i <= endHourNumber; ++i)
	{
		mHoursComboBox->addItem(QString::number(i) + QString{" hrs"});
	}

	for (int i = 0; i < 4; ++i)
	{
		mMinutesComboBox->addItem(QString::number(i * 15) + QString{" mnts"});
	}
	chooseLayout->addWidget(mHoursComboBox);
	chooseLayout->addWidget(mMinutesComboBox);

	mMainLayout->addWidget(mChooseDateCalendar);
	mMainLayout->addLayout(chooseLayout);
	mMainLayout->addWidget(mStartPushButton);
	mMainLayout->addWidget(mAddTimePushButton);
	setLayout(mMainLayout);
}

void DayStartPage::handleStartButtonEvent()
{
	getContext().mainWindow->selectPage(MainWindow::Pages::WorkDay);
}

void DayStartPage::setupCoreWidgetsConnections()
{
	connect(mChooseDateCalendar, &QCalendarWidget::selectionChanged, this,
		[this]()
		{
			//
			const QDate& date = mChooseDateCalendar->selectedDate();
			// If our workday already been started before
			if (DatabaseManager::instance().isDateExist(date))
			{
				WorkDay wDay = DatabaseManager::instance().getWorkDay(date);
				int		goalTotalMinutes = wDay.getGoal();
				int		hrsIndx = hoursToHoursIndx(goalTotalMinutes / 60);
				int		mntsIndx = minutesToMinutesIndx(goalTotalMinutes % 60);
				mHoursComboBox->setCurrentIndex(hrsIndx);
				mMinutesComboBox->setCurrentIndex(mntsIndx);

				mHoursComboBox->setEnabled(false);
				mMinutesComboBox->setEnabled(false);

				mStartPushButton->setText("continue");
				mAddTimePushButton->setVisible(true);
				mIsDayAlreadyStarted = true;
			}
			else
			{
				mHoursComboBox->setCurrentIndex(0);
				mMinutesComboBox->setCurrentIndex(0);

				mHoursComboBox->setEnabled(true);
				mMinutesComboBox->setEnabled(true);

				mStartPushButton->setText("start");
				mAddTimePushButton->setVisible(false);
				mIsDayAlreadyStarted = false;
			}
		});

	connect(mAddTimePushButton, &QPushButton::pressed, this,
		[this]()
		{
			assert((mIsDayAlreadyStarted == true) && "Error");
			AddTimeDialog addTimeDialog{this};
			auto		  result = addTimeDialog.exec();
			if (result == QDialog::Accepted)
			{
				int oldGoalTotalMinutes = getSelectedTotalMinutes();
				int additionalMinutes = addTimeDialog.getTotalMinutes();

				int resultGoalTotalMinutes = oldGoalTotalMinutes + additionalMinutes;
				int hrs = resultGoalTotalMinutes / 60;
				int mnts = resultGoalTotalMinutes % 60;

				int hrsIndx = hoursToHoursIndx(hrs);
				int mntsIndx = minutesToMinutesIndx(mnts);

				mHoursComboBox->setCurrentIndex(hrsIndx);
				mMinutesComboBox->setCurrentIndex(mntsIndx);
			}
		});

	connect(mStartPushButton, &QPushButton::pressed, getContext().mainWindow,
		[this]()
		{
			int hrsGoal = hoursIndxToHours(mHoursComboBox->currentIndex());
			int mntsGoal = minutesIndxToMinuts(mMinutesComboBox->currentIndex());
			int goalTotalMinutes = hrsGoal * 60 + mntsGoal;

			if (!mIsDayAlreadyStarted)
			{
				int ret = QMessageBox::information(this, tr("work_day"),
					tr("Goal time for today day is %1:%2 (%3 min)")
						.arg(hrsGoal)
						.arg(QString::number(mntsGoal).rightJustified(2, '0'))
						.arg(hrsGoal * 60 + mntsGoal),
					QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

				if (ret == QMessageBox::Ok)
				{
					auto date = mChooseDateCalendar->selectedDate();

					addProgDataEntry(
						Page::ProgDataType::GoalTotalMinutes, goalTotalMinutes);
					addProgDataEntry(
						Page::ProgDataType::TotalMinutesLeftAtStart, goalTotalMinutes);
					addProgDataEntry(Page::ProgDataType::Date, date);

					DatabaseManager::instance().addWorkDay(
						WorkDay{date, hrsGoal * 60 + mntsGoal});
					getContext().mainWindow->selectPage(MainWindow::Pages::WorkDay);
				}
			}
			else
			{
				auto	date = mChooseDateCalendar->selectedDate();
				WorkDay wDay = DatabaseManager::instance().getWorkDay(date);
				int		curResultTotalMinutes = wDay.getResult();
				int		goalTotalMinutes = hrsGoal * 60 + mntsGoal;
				int		minutesLeft = goalTotalMinutes - curResultTotalMinutes;
				int		hrsLeft = minutesLeft / 60;
				int		mntsLeft = minutesLeft % 60;

				qDebug() << goalTotalMinutes << " " << minutesLeft;

				assert((minutesLeft >= 0) && "Error");

				addProgDataEntry(Page::ProgDataType::GoalTotalMinutes, goalTotalMinutes);
				addProgDataEntry(
					Page::ProgDataType::TotalMinutesLeftAtStart, minutesLeft);
				addProgDataEntry(Page::ProgDataType::Date, date);

				DatabaseManager::instance().updateWorkDay(
					WorkDay{date, goalTotalMinutes, hrsLeft * 60 + mntsLeft});
				getContext().mainWindow->selectPage(MainWindow::Pages::WorkDay);
			}
		});
}

int DayStartPage::getSelectedTotalMinutes() const
{
	int hrs = hoursIndxToHours(mHoursComboBox->currentIndex());
	int mnts = minutesIndxToMinuts(mMinutesComboBox->currentIndex());

	return hrs * 60 + mnts;
}
