#ifndef RSAPUBLICKEY_H
#define RSAPUBLICKEY_H
#include "config.h"

#ifdef HAVE_CRYPTO

#include "RawRsaPublicKey.h"
#include "RawData.h"

class RsaPublicKey : public RawRsaPublicKey {
	Q_DECLARE_TR_FUNCTIONS(RsaPublicKey)
public:
	RsaPublicKey(const DataPointer& start);
	QByteArray perform(const RawData& input) const;
};

#else

#include "RawData.h"
class RsaPublicKey : public DataType {
public:
	RsaPublicKey(const DataPointer& start) : DataType(start) {}
	int size() const { return 128+8; }
	QString className() const { return "RsaPublicKey"; }
	void printOn(Reporter& report) const{
	}
};

#endif

#endif
