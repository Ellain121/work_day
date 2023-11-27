#include "DayEndPage.hpp"

#include "DatabaseManager.hpp"
#include "MainWindow.hpp"

#include <QMessageBox>

DayEndPage::DayEndPage(Page::Context context, QWidget* parent)
	: Page{context, parent}
{
	mDateString = getProgDataEntry(Page::ProgDataType::Date).toString();
	mWorkDoneString = getProgDataEntry(Page::ProgDataType::WorkDone).toString();
	mDateLabel = new QLabel{"date: " + mDateString, this};
	mWorkDoneLabel = new QLabel{"time: " + mWorkDoneString, this};

	setupCoreWidgets();
	setupCoreWidgetsConnections();
}

void DayEndPage::setupCoreWidgets()
{
	mMainLayout = new QVBoxLayout{this};
	QHBoxLayout* layout_1 = new QHBoxLayout{this};
	mTextEdit = new QTextEdit{this};
	mSendReportButton = new QPushButton{"send report", this};

	setLayout(mMainLayout);

	layout_1->addWidget(mDateLabel);
	layout_1->addWidget(mWorkDoneLabel);

	mMainLayout->addLayout(layout_1);
	mMainLayout->addWidget(mTextEdit);
	mMainLayout->addWidget(mSendReportButton);
}

void DayEndPage::setupCoreWidgetsConnections()
{
	connect(mSendReportButton, &QPushButton::pressed, this, &DayEndPage::sendReport);
}

void DayEndPage::sendReport()
{
	int ret = QMessageBox::information(this, tr("work_day"),
		tr("Are you sure you want to end the work day?"),
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
	if (ret == QMessageBox::Ok)
	{
		getContext().mainWindow->endProgram();
		//		DatabaseManager::instance().updateWorkDay();
	}
}
