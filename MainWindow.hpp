#pragma once

#include "Page.hpp"

#include <QMainWindow>
#include <QMap>
#include <QStackedWidget>
#include <QVariant>

class QApplication;

class DayStartPage;
class DayWorkPage;
class DayEndPage;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	enum class Pages
	{
		Start = 0,
		WorkDay,
		End,
	};

	enum class Icon
	{
		BMO_Bright,
		BMO_Dark,
		BMO_Orange
	};

public:
	MainWindow(QApplication* app, QWidget* parent = 0);

	void selectPage(
		Pages pageID, QMap<QString, QVariant> pageData = QMap<QString, QVariant>());
	void endProgram();

	void setWindowIcon(Icon icon);

private:
	void setupCoreWidgets();
	void setupCoreWidgetsConnections();

private:
	QApplication*	  mApplication;
	QStackedWidget*	  mMainStackedWidgets;
	DayStartPage*	  mDayStartPage;
	DayWorkPage*	  mDayWorkPage;
	DayEndPage*		  mDayEndPage;
	Page::ProgramData mProgramData;
	QIcon			  mBMO_Bright;
	QIcon			  mBMO_Dark;
	QIcon			  mBMO_Orange;
};
