#include "MainWindow.hpp"

#include "DayEndPage.hpp"
#include "DayStartPage.hpp"
#include "DayWorkPage.hpp"
#include "Utility.hpp"

#include <QApplication>

MainWindow::MainWindow(QApplication* app, QWidget* parent)
	: QMainWindow{parent}
	, mApplication{app}
	, mMainStackedWidgets{new QStackedWidget}
	, mDayStartPage{nullptr}
	, mDayWorkPage{nullptr}
	, mDayEndPage{nullptr}
	, mBMO_Bright{QPixmap::fromImage(QImage{":/bmo_bright.png"})}
	, mBMO_Dark{QPixmap::fromImage(QImage{":/bmo_dark.png"})}
	, mBMO_Orange{QPixmap::fromImage(QImage{":/bmo_orange.jpg"})}
{
	setWindowIcon(Icon::BMO_Dark);

	setupCoreWidgets();
	setupCoreWidgetsConnections();

	writeLog("Application started");
}

void MainWindow::selectPage(Pages pageID, QMap<QString, QVariant> pageData)
{
	if (pageID == Pages::WorkDay)
	{
		if (mDayWorkPage == nullptr)
		{
			mDayWorkPage = new DayWorkPage{Page::Context{*this, mProgramData}};
			mMainStackedWidgets->addWidget(mDayWorkPage);
		}
		mMainStackedWidgets->setCurrentWidget(mDayWorkPage);
		writeLog("WorkDayPage has been open");
	}
	else if (pageID == Pages::End)
	{
		if (mDayEndPage == nullptr)
		{
			mDayEndPage = new DayEndPage{Page::Context{*this, mProgramData}};
			mMainStackedWidgets->addWidget(mDayEndPage);
		}
		mMainStackedWidgets->setCurrentWidget(mDayEndPage);
		writeLog("EndDayPage has been open");
	}
	else
	{
		assert("Error: Uknown pageID!");
	}
}

void MainWindow::endProgram()
{
	mApplication->quit();
}

void MainWindow::setWindowIcon(Icon icon)
{
	switch (icon)
	{
		case Icon::BMO_Bright:
			QMainWindow::setWindowIcon(mBMO_Bright);
			break;

		case Icon::BMO_Dark:
			QMainWindow::setWindowIcon(mBMO_Dark);
			break;

		case Icon::BMO_Orange:
			QMainWindow::setWindowIcon(mBMO_Orange);
			break;
	}
}

void MainWindow::setupCoreWidgets()
{
	assert((mDayStartPage == nullptr) && "mDayStartPage must be nullptr at the start");

	mDayStartPage = new DayStartPage{Page::Context{*this, mProgramData}};
	mMainStackedWidgets->addWidget(mDayStartPage);
	mMainStackedWidgets->setCurrentWidget(mDayStartPage);

	setCentralWidget(mMainStackedWidgets);
}

void MainWindow::setupCoreWidgetsConnections()
{
}
