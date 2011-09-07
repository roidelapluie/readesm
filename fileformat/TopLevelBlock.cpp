#include "TopLevelBlock.h"

#include "config.h"

RawData TopLevelBlock::signatureBytes() const {
	return RawData(start + size() - 128, 128);
}

TopLevelBlock::TopLevelBlock(const DataPointer& start) : Block(start),
	validSignature(false),
	hasSignature(false)
{
}

void TopLevelBlock::checkSignature(const EncryptedCertificate& cert){
#ifdef HAVE_CRYPTO
	if(hasSignature) validSignature = cert.checkSignature(signedBytes(), signatureBytes());
#endif
}

QString TopLevelBlock::signatureValidity() const{
#ifdef HAVE_CRYPTO
	if(hasSignature) {
		if(validSignature) return tr("Block has valid signature");
		else return tr("Beware: Block has invalid signature");
	} else return tr("Block not signed");
#else
	return tr("no crypto support compiled in");
#endif
}

Reporter& operator<<(Reporter& report, const TopLevelBlock& b){
	b.printOn(report);
	report.tagValuePair(QObject::tr("signature"), b.signatureValidity());
	return report;
}

void TopLevelBlock::printOn(Reporter& o) const {
	o.tagValuePair("length", size());
}


QString TopLevelBlock::name() const {
	return tr("Unknown block type 0x%1").arg(readBigEndianInt2(start), 8, 16, QChar('0'));
}
