#include "CardBlock.h"

CardBlock::CardBlock(const constDataPointer& filewalker) :
	Block(filewalker),
	datasize(readBigEndianInt2(start + 3))
{
	constDataPointer signature = start + 5 + datasize;
	if(signature.bytesLeft() >= 5 + 128 && readBigEndianInt2(signature) == readBigEndianInt2(start) && signature[2] == 1) {
		hasSignature = true;
	}
}

RawData CardBlock::signedBytes() const {
	return RawData(start + 5, dataSize());
}

int  CardBlock::dataSize() const {
	return datasize;
}

int  CardBlock::size() const {
	return datasize + 5 + (hasSignature ? 5 + 128 : 0);
}
