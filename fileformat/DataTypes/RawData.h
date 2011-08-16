#ifndef RAWDATA_H
#define RAWDATA_H

#include "../constDataPointer.h"

#include <QtCore/QString>

class RawData : public constDataPointer {
public:
	int length;
	RawData(const constDataPointer& start, int length_) : constDataPointer(start), length(length_) {}
	QString toString() const{
		QString rv;
		for(int j = 0; j < length; ++j) rv.append(QString("%1 ").arg(operator[](j), 2, 16, QChar('0')));
		return rv;
	}
};

#endif
