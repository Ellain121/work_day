#pragma once

#include "Page.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class DayEndPage : public Page
{
    Q_OBJECT

public:
    DayEndPage(Page::Context context, QWidget* parent = nullptr);

private:
    void setupCoreWidgets();
    void setupCoreWidgetsConnections();

private slots:
    void sendReport();

private:
    QVBoxLayout* mMainLayout;

    QString mDateString;
    QLabel* mDateLabel;
    QString mWorkDoneString;
    QLabel* mWorkDoneLabel;

    QTextEdit*	 mTextEdit;
    QPushButton* mSendReportButton;
};
