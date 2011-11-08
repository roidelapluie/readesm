#ifndef CARDBLOCK_H
#define CARDBLOCK_H

#include "../TopLevelBlock.h"

class CardBlock : public TopLevelBlock {
	public:
	CardBlock(const DataPointer& filewalker);
	int size() const;

	///size of Data block (i.e. without signature)
	int dataBlockSize() const;

	QString className() const;
	
	virtual RawData signedBytes() const;

	protected:
	int datasize;

};

#endif
