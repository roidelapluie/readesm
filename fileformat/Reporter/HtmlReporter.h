#ifndef HTMLREPORTER_H
#define HTMLREPORTER_H

#include "Reporter.h"

class HtmlReporter : public Reporter {
public:
	virtual void tagValuePair(const QString& tag, const QString& value);
	virtual void namedSubBlock(const QString& tag, const DataType& value);
	virtual void topLevelBlock(const TopLevelBlock& value);
	QByteArray toQByteArray() const;
	virtual bool allowSvg() const;

};


#endif
