#include "loadFile.h"

#include <QtCore/QFile>

QByteArray loadFile(const QString& filename){
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly));
	QByteArray content = file.readAll();
	file.close();
	return content;
}
