#include "Block.h"

RawData Block::signatureBytes() const {
	return RawData(start + size() - 128, 128);
}