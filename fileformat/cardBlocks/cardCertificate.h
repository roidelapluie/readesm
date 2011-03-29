#ifndef CARDCERTIFICATE_H
#define CARDCERTIFICATE_H

#include "cardBlock.h"

class cardCertificate : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(cardCertificate)
	public:
	static const int Type = 0xc100;
	virtual QString name() const {
		return tr("Card Certificate");
	}
	cardCertificate(constDataPointer filewalker) :
		cardBlock(filewalker) {
	}
	virtual void reportstuff(esmfilehead& esm) {
#ifndef HAVE_NO_CRYPTO
		if(esm.devicecert) qDebug() << "Reassigning cert";
		esm.devicecert = verifiedcert::ptr(new verifiedcert(start + 5));
#endif
	}
};

#endif
