#include "constDataPointer.h"

#include <QtCore/QFile>

constDataPointer constDataPointer::loadFile(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly));
	QByteArray content = file.readAll();
	file.close();
	return content;
}
QByteArray constDataPointer::toQByteArray() const{
	return data; 
}
