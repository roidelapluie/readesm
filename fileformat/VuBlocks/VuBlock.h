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

#ifndef VUBLOCK_H
#define VUBLOCK_H

#include "../Block.h"
#include "../constDataPointer.h"

#include <QtCore/QString>

class VuBlock : public Block {
	public:
	virtual QString name() const = 0;
	VuBlock(const constDataPointer& nstart);
	virtual int size() const = 0;
	virtual void printOn(reporter& report) const = 0;
	virtual RawData signedBytes() const;
};

#endif
