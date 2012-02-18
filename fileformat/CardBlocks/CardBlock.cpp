#include "CardBlock.h"

CardBlock::CardBlock(const DataPointer& filewalker) :
	TopLevelBlock(filewalker),
	datasize(readBigEndianInt2(start + 3))
{
	DataPointer signature = start + 5 + datasize;
	if(signature.bytesLeft() >= 5 + 128 && readBigEndianInt2(signature) == readBigEndianInt2(start) && signature[2] == 1) {
		hasSignature = true;
	}
}

RawData CardBlock::signedBytes() const {
	return RawData(start + 5, dataBlockSize() - 5);
}

int CardBlock::dataBlockSize() const {
	return datasize + 5;
}

int CardBlock::size() const {
	return datasize + 5 + (hasSignature ? 5 + 128 : 0);
}

QString CardBlock::className() const {
	return "CardBlock";
}
