#pragma once

#include "Page.hpp"

#include <QCalendarWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class DayStartPage : public Page
{
	Q_OBJECT

public:
	DayStartPage(Page::Context context, QWidget* parent = nullptr);

private:
	void setupCoreWidgets();
	void setupCoreWidgetsConnections();

	int getSelectedTotalMinutes() const;

private slots:
	void handleStartButtonEvent();

private:
	QVBoxLayout*	 mMainLayout;
	QComboBox*		 mHoursComboBox;
	QComboBox*		 mMinutesComboBox;
	QPushButton*	 mStartPushButton;
	QPushButton*	 mAddTimePushButton;
	QCalendarWidget* mChooseDateCalendar;
	bool			 mIsDayAlreadyStarted;
};
