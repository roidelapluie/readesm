#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "RawData.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

class Signature : public RawData {
	Q_DECLARE_TR_FUNCTIONS(Signature)
	bool checked;
	bool valid;
public:
	Signature(const constDataPointer& start);
	QString toString() const;
};

#endif