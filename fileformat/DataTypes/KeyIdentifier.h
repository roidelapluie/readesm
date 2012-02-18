#ifndef KEYIDENTIFIER_H
#define KEYIDENTIFIER_H

#include "DataType.h"

#include <QtCore/QSharedPointer>

class KeyIdentifier : public DataType {
	Q_DECLARE_TR_FUNCTIONS(KeyIdentifier)
public:
	KeyIdentifier(const DataPointer& filewalker);
	void printOn(Reporter& o) const;
	QString className() const;
	int size() const;
protected:
	QSharedPointer<DataType> content;
};


#endif
