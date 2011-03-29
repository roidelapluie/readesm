#ifndef CACERTIFICATE_H
#define CACERTIFICATE_H

#include "cardCertificate.h"

class caCertificate : public cardCertificate {
	Q_DECLARE_TR_FUNCTIONS(caCertificate)
	public:
	static const int Type = 0xc108;
	virtual QString name() const {
		return tr("CA Certificate");
	}
	caCertificate(constDataPointer filewalker) :
		cardCertificate(filewalker) {
	}
	virtual void reportstuff(esmfilehead& esm) {
#ifndef HAVE_NO_CRYPTO
		if(esm.CAcert) qDebug() << "Reassigning cert\n";
		esm.CAcert = verifiedcert::ptr(new verifiedcert(start + 5));
#endif
	}
};

#endif
