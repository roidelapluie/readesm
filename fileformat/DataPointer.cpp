#include "DataPointer.h"

#include "loadFile.h"

#include <QtCore/QFile>

QByteArray DataPointer::toQByteArray() const{
	return data;
}
