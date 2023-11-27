#include "AddTimeDialog.hpp"

#include "Utility.hpp"

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

AddTimeDialog::AddTimeDialog(QWidget* parent)
	: QDialog{parent}
	, mFormLayout{new QFormLayout{this}}
	, mHoursComboBox{new QComboBox{this}}
	, mMinutesComboBox{new QComboBox{this}}
{
	for (int i = 0; i < 6; ++i)
	{
		mHoursComboBox->addItem(QString::number(i));
	}
	for (int i = 0; i < 4; ++i)
	{
		mMinutesComboBox->addItem(QString::number(i * 15));
	}

	QLabel* headerLabel = new QLabel{"Add time", this};
	headerLabel->setAlignment(Qt::AlignCenter);
	mFormLayout->addRow(headerLabel);
	mFormLayout->addRow(tr("Add hours: "), mHoursComboBox);
	mFormLayout->addRow(tr("Add minutes: "), mMinutesComboBox);

	// Add some standard buttons (Cancel/Ok) at the bottom of the dialog
	QDialogButtonBox* buttonBox = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	mFormLayout->addRow(buttonBox);

	/*************** [Connections] ************************/
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

int AddTimeDialog::getTotalMinutes() const
{
	int hrs = toInt(mHoursComboBox->currentText());
	int mnts = toInt(mMinutesComboBox->currentText());

	return hrs * 60 + mnts;
}
