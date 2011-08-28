#include "VuBlock.h"

RawData VuBlock::signedBytes() const {
	return RawData(start + 2, size() - 128 - 2);
}

VuBlock::VuBlock(const DataPointer& nstart) :
	TopLevelBlock(nstart)
{
	hasSignature = true;
}
