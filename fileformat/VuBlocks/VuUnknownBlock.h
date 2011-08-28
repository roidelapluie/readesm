#ifndef VUUNKNOWNBLOCK_H
#define VUUNKNOWNBLOCK_H

#include "VuBlock.h"

/** This block is for debugging, in case the program encounters
 * some data it so far cannot read, it reports hex data and as
 * many useful information as possible **/
class VuUnknownBlock : public VuBlock {
	public:
	RawData data;
	VuUnknownBlock(const DataPointer& start);

	virtual int size() const;

	virtual QString name() const;
	
	virtual void printOn(Reporter& o) const;

};

#endif
