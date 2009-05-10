/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef BLOCK_H
#define BLOCK_H BLOCK_H
#include "helper.h"
#include "esmfilehead.h"

class block{
	public:
	typedef boost::shared_ptr<block> ptr;
	block(iter filewalker) : 
		validsignature(false), 
		hassignature(false), 
		start(filewalker), 
		type(getType(start)) {}
	static int getType(iter filewalker){
		return (filewalker[0] << 8) + filewalker[1];
	}
	friend std::ostream& operator<< (reporter& o, const block& b){
		o.bigblockstart(b.name());
		b.printOn(o);
		o("validly signed",b.hassignature? (b.validsignature ? "yes, valid" : "no, invalid") : "no, not signed");
		o.bigblockend();
		return o;
	}
	virtual int size() const = 0;
	virtual string name() const{
		return "Unknown block type " +  hex(type,4);
	}
	virtual void reportstuff(esmfilehead& esm){}
	static ptr Factory(iter& filewalker);
	virtual bool checksig(const rsa& key) = 0;
	protected:
	virtual void printOn(reporter& o) const{
		o("length", size());
	}
	//virtual void printLess(reporter& o) const{}
	protected:
	iter signature;
	bool validsignature, hassignature;
	iter start;
	int type;
};

#endif
