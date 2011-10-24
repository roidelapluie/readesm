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
class Block;

class Reporter {
	Q_DECLARE_TR_FUNCTIONS(Reporter)
public:
	virtual void tagValuePair(const QString& tag, const QString& value) = 0;
	virtual void tagValuePair(const QString& tag, int value);
	virtual void writeBlock(const Block& value, const QString& tag = "");
	///return the qbytearray that contains the report, ready for being written to a file.
	virtual QByteArray toQByteArray() const = 0;
	virtual void setTitle(const QString& newtitle);
	virtual bool allowSvg() const = 0;
	void flush();
	Reporter();
	template <typename Arraytype>
	void writeArray(const Arraytype& ray, const QString& title = "", bool defaultShown = true){
		arrayStart(ray.numberOfBlocks(), title, defaultShown);
		++nestLevel;
		for(int j = 0; j < ray.numberOfBlocks(); ++j){
			this->writeBlock(ray[j]);
		}
		--nestLevel;
		arrayEnd(ray.numberOfBlocks());
	}
protected:
	int nestLevel;
	QByteArray collected;
	mutable QTextStream collector;
	QString title;
	virtual void subBlock(const Block& value, const QString& tag) = 0;
	virtual void arrayStart(int count, const QString& title, bool defaultShown){}
	virtual void arrayEnd(int count){}
};

#endif
