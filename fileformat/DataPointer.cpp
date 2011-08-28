#include "DataPointer.h"

#include <QtCore/QFile>
#include "loadFile.h"


QByteArray DataPointer::toQByteArray() const{
	return data; 
}
