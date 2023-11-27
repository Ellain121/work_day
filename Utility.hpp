#pragma once

#include <QDate>
#include <QString>
#include <QVariant>
#include <source_location>

using std::source_location;
using std::string_view;

int toInt(
	const QVariant& var, const source_location& location = source_location::current());

QDate toDate(const QVariant&	var,
	const std::source_location& location = source_location::current());

QString toQString(
	const QVariant& var, const source_location& location = source_location::current());

int toInt(
	const QString& var, const source_location& location = source_location::current());

void logMessage(
	string_view msg, const source_location& location = source_location::current());

void writeLog(string_view msg);
