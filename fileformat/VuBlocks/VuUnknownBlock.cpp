#include "VuUnknownBlock.h"

VuUnknownBlock::VuUnknownBlock(const DataPointer& start) :
	VuBlock(start), data(start, start.bytesLeft())
{
	hasSignature = false;
}

int VuUnknownBlock::size() const {
	return start.bytesLeft();
}

QString VuUnknownBlock::title() const {
	return tr("Unknown Vu block, TREP %1").arg(hexByte(start[1]));
}

QString VuUnknownBlock::className() const {
	return tr("VuUnknownBlock%1").arg(hexByte(start[1]));
}


void VuUnknownBlock::printOn(Reporter& o) const {
	o.writeBlock(data, "Raw Data");
/*	o.single("Oops! readesm encountered a chunk of data it cannot read.  Since the length of this chunk is also unknown, it cannot read the remaining file.");
	o("Block offset", QString("%1 (0x%2)")
				.arg(start.offset)
				.arg(start.offset, 8, 16, QChar('0')));
	o("Block size", QString("%1").arg(size()));
	o.single("If you are convinced readesm should be able to read this file, inform the author");
	o.single("Block hex data: " + data.toString());*/
}
