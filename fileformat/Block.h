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

#ifndef BLOCK_H
#define BLOCK_H BLOCK_H

#include "DataPointer.h"
#include "readTypes.h"
#include "Reporter/Reporter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

class Block {
	Q_DECLARE_TR_FUNCTIONS(Block)
	public:
	Block(const DataPointer& filewalker);
	virtual int size() const = 0;
	friend Reporter& operator<<(Reporter& o, const Block& b);
	
	protected:
	virtual void printOn(Reporter& o) const = 0;
	DataPointer start;
};

#endif
