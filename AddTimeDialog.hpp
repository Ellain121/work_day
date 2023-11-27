#pragma once

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>

class AddTimeDialog : public QDialog
{
	Q_OBJECT

public:
	AddTimeDialog(QWidget* parent = nullptr);

	int getTotalMinutes() const;

private:
	QFormLayout* mFormLayout;
	QComboBox*	 mHoursComboBox;
	QComboBox*	 mMinutesComboBox;
};
