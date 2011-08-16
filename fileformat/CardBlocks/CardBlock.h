#ifndef CARDBLOCK_H
#define CARDBLOCK_H

#include "../Block.h"

class CardBlock : public Block {
	public:
	CardBlock(const constDataPointer& filewalker) :
		Block(filewalker),
		datasize(readBigEndianInt2(start + 3))
	{
		signature = start + 5 + datasize;
		if(signature.bytesLeft() >= 5 + 128 && readBigEndianInt2(signature) == readBigEndianInt2(start) && signature[2] == 1) {
			signature += 5;
			hassignature = true;
		}
	}
	int size() const {
		return datasize + 5 + (hassignature ? 5 + 128 : 0);
	}
	int dataSize() const {
		return datasize;
	}
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa& key) {
		if(hassignature) validsignature = CheckSignature(start + 5, datasize,
				signature, 128, key);
		return validsignature;
	}
#endif
	protected:
	int datasize;

};

#endif
