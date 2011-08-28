/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef REPORTER_H
#define REPORTER_H REPORTER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QTextStream>

class DataType;
class TopLevelBlock;

class Reporter {
	Q_DECLARE_TR_FUNCTIONS(Reporter)
public:
	virtual void tagValuePair(const QString& tag, const QString& value) = 0;
	virtual void tagValuePair(const QString& tag, int value);
	virtual void namedSubBlock(const QString& tag, const DataType& value) = 0;
	virtual void topLevelBlock(const TopLevelBlock& value) = 0;
	virtual QByteArray toQByteArray() const = 0;
	virtual void setTitle(const QString& newtitle);
	void flush();
	Reporter();
protected:
	QByteArray collected;
	QTextStream collector;
	QString title;
};

#endif
