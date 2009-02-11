/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef VUBLOCK_H
#define VUBLOCK_H
#include <string>
#include "time.h"
#include "crypto.h"
#include "block.h"
#include "readTypes.h"


class vublock  : public block {
	public:
	virtual std::string name() const = 0;
	vublock(iter nstart) : block(nstart), block_start(&nstart[2]), runningIndex(0) {
		hassignature = true;
	}
	virtual void Init(){ signature = start + size() - 128; }
	int getBEInt32(const unsigned char* start) const{
		return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3]; 
	}
	Time readDate(const unsigned char* in) const{
		return Time::fromBigEndianChars32(in);
	}

	Time readDate(int start) const{
		return readDate(&block_start[start]);
	}
	Time readDate() const{
		runningIndex += 4;
		return readDate(runningIndex - 4);
	}
	std::string fixedString(int length) const{
		runningIndex += length;
		return ::fixedString(start + 2 + runningIndex - length, length);
	}
	int IntByte(int start) const{
		return int(block_start[start]);
	}
	int IntByte() const{
		runningIndex += 1;
		return IntByte(runningIndex - 1);
	}
	int Int16(int start) const{
		return (block_start[start] << 8)+ block_start[start+1];
	}
	int Int16() const{
		runningIndex += 2;
		return Int16(runningIndex - 2);
	}

	int Odometer(int start) const{
		return (block_start[start] << 16) + (block_start[start + 1] << 8) + block_start[start + 2];
	}
	int Odometer() const{
		runningIndex += 3;
		return Odometer(runningIndex - 3);
	}

	std::string fixedString(int start, int length) const{
		std::ostringstream o;
		//for(int j = start; j < start + length; ++j) if(block_start[j] >= 0x20 && block_start[j] < 127) 
		//o << block_start[j];
		for(int j = start; j < start + length; ++j) if(block_start[j] >= 0x20) o << block_start[j];
		//files seem to be latin-1, watch out
		return o.str();
	}
	const unsigned char* block_start;
	mutable int runningIndex;
	virtual int size() const = 0;
	virtual void CompleteReport(reporter& report) const = 0;
	virtual void BriefReport(reporter& report) const = 0;
	virtual void printOn(reporter& report) const{
		if(report.verbose) CompleteReport(report);
		else BriefReport(report);
	}
	virtual bool checksig(const rsa& key){;
		validsignature = CheckSignature(start + 2 + nonhashedbytes(), size() - 128 - 2 - nonhashedbytes(), signature, 128, key);
		return validsignature;
	}
	virtual int nonhashedbytes() const{ return 0;}
};

#endif
