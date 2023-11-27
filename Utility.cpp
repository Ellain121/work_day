#include "Utility.hpp"

#include <QDateTime>
#include <format>
#include <fstream>
#include <iostream>

using std::format;

namespace
{
std::string	  logFile = "log.txt";
std::ofstream outFile{logFile};
}	 // namespace

int toInt(const QVariant& var, const source_location& location)
{
	bool ok = false;
	int	 intValue = var.toInt(&ok);
	if (!ok)
	{
		logMessage("Failed conversion: QVariant to int", location);
	}
	return intValue;
}

QDate toDate(const QVariant& var, const std::source_location& location)
{
	if (!var.canConvert<QDate>())
	{
		logMessage("Failed conversion: QVariant to QDate", location);
	}
	return var.toDate();
}

QString toQString(const QVariant& var, const std::source_location& location)
{
	if (!var.canConvert<QString>())
	{
		logMessage("Failed conversion: QVariant to QString", location);
	}
	return var.toString();
}

int toInt(const QString& var, const std::source_location& location)
{
	bool ok = false;
	int	 intValue = var.toInt(&ok);
	if (!ok)
	{
		logMessage("Failed conversion: QVariant to int", location);
	}
	return intValue;
}

void logMessage(string_view msg, const source_location& location)
{
	std::cout << format("{}({}): {}: {}", location.file_name(), location.line(),
					 location.function_name(), msg)
			  << std::endl;
}

void writeLog(string_view msg)
{
	outFile
		<< QDateTime::currentDateTime().toString("ddd MMMM d yyyy [hh:mm]").toStdString()
		<< ": " << msg << std::endl;
}
