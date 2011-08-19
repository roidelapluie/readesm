#ifndef RSAPUBLICKEY_H
#define RSAPUBLICKEY_H

#include "RawRsaPublicKey.h"
#include "RawData.h"

class RsaPublicKey : public RawRsaPublicKey {
	Q_DECLARE_TR_FUNCTIONS(RsaPublicKey)
public:
	RsaPublicKey(const constDataPointer& start);
	QByteArray perform(const RawData& input) const;
};
#endif
