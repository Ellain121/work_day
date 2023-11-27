#pragma once

#include "Page.hpp"
#include "Timer.hpp"

#include <QDate>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class DayWorkPage : public Page
{
	Q_OBJECT

public:
	DayWorkPage(Page::Context context, QWidget* parent = nullptr);

private:
	void setupCoreWidgets();
	void setupCoreWidgetsConnections();

private slots:
	void timeIsUp();
	void proceed();
	void pause();

private:
	QDate		 mDate;
	Timer*		 mTimer;
	QVBoxLayout* mMainLayout;

	QPushButton* mPauseButton;
	QPushButton* mContinueButton;
	QPushButton* mFinishEarlyButton;
	QPushButton* mFinishButton;
	QPushButton* mInfoButton;
	QPushButton* mAddExtraTimeButton;
};
