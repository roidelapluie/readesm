#ifndef RAWDATA_H
#define RAWDATA_H

#include "../constDataPointer.h"

#include <QtCore/QString>

class RawData : public constDataPointer {
public:
	int length_;
	RawData(const constDataPointer& start, int length);
	RawData(const QByteArray& o);
	QString toString() const;
	int length() const;
	QByteArray toQByteArray() const;
};

#endif
