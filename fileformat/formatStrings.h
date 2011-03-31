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

#ifndef FORMATSTRINGS_H
#define FORMATSTRINGS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

class formatStrings {
	Q_DECLARE_TR_FUNCTIONS(formatStrings)

	public:

	///What kind of event(mostly "driving without the right card")
	static QString eventType(unsigned char etype);

	///Why was the event recorded?
	static QString eventRecordPurpose(unsigned char etype);

	///Which country in the EU?
	static QString nationNumeric(unsigned char country);

	///Which region in Spain? (seems to be only relevant there)
	static QString regionNumeric(unsigned char region);

	static QString specificCondition(unsigned char scond);

	///Why was the VU (re)calibrated?
	static QString calibrationPurpose(unsigned char cpurp);

	///Control type(printing, vu read, ...)
	static QString controlType(unsigned char ctype);

	///Kind of card(control, driver...)
	static QString equipmentType(unsigned char value);

	static QString manufacturerCode(unsigned char code);

	static QString dailyWorkPeriod(unsigned char value);
};
#endif
