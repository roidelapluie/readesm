#ifndef PLACES_H
#define PLACES_H

#include "cardBlock.h"

#include "../dataTypes/placeRecord.h"

class places : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(places)
	public:
	typedef std::vector<placeRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0506;
	virtual QString name() const {
		return tr("Places");
	}
	places(constDataPointer filewalker) :
		cardBlock(filewalker) 
	{
		for(constDataPointer i = start + 5 + 1; i < start + 5 + datasize; i += 10) {
			if(!placeRecord::defval(i)) sub.push_back(placeRecord(i));
		}
	}
	virtual void printOn(reporter& o) const {
		o.reportraynosub(sub);
	}
};

#endif
