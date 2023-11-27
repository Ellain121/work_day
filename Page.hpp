#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QWidget>

class MainWindow;

class Page : public QWidget
{
	Q_OBJECT

public:
	enum class ProgDataType
	{
		GoalTotalMinutes = 0,
		Date,
		WorkDone,
		TotalMinutesLeftAtStart
	};

public:
	typedef QMap<ProgDataType, QVariant> ProgramData;

	struct Context
	{
		Context(MainWindow& mWindow, ProgramData& progData);

		MainWindow*	 mainWindow = nullptr;
		ProgramData* progData = nullptr;
	};

public:
	Page(Context context, QWidget* parent = nullptr);

protected:
	Context	 getContext() const;
	void	 addProgDataEntry(ProgDataType type, const QVariant& var);
	void	 updateProgDataEntry(ProgDataType type, const QVariant& var);
	QVariant getProgDataEntry(ProgDataType type) const;

private:
	Context mContext;
};
