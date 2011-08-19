#ifndef CARDBLOCK_H
#define CARDBLOCK_H

#include "../Block.h"

class CardBlock : public Block {
	public:
	CardBlock(const constDataPointer& filewalker);
	int size() const;
	int dataSize() const;
	
	virtual RawData signedBytes() const;

	protected:
	int datasize;

};

#endif
