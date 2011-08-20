#include "Block.h"

#include "config.h"

RawData Block::signatureBytes() const {
	return RawData(start + size() - 128, 128);
}

Block::Block(const constDataPointer& filewalker) :
	validSignature(false),
	hasSignature(false),
	start(filewalker)
{
}


reporter& operator<<(reporter& o, const Block& b) {
	o.bigblockstart(b.name());
	b.printOn(o);
#ifdef HAVE_CRYPTO
	o.single(b.hasSignature ? (b.validSignature
			? QObject::tr("Block has valid signature") : QObject::tr("Beware: Block has invalid signature")) : QObject::tr("Block not signed"));
#else
	o.single(QObject::tr("no crypto support compiled in"));
#endif
	o.bigblockend();
	return o;
}

void Block::printOn(reporter& o) const {
	o("length", size());
}


QString Block::name() const {
	return tr("Unknown block type 0x%1").arg(readBigEndianInt2(start), 8, 16, QChar('0'));
}
