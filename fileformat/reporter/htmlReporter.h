#ifndef HTMLREPORTER_H
#define HTMLREPORTER_H

#include "reporter.h"

#include <QtCore/QString>
#include <QtCore/QTextStream>

///HTML reporter
/** This reporter aims to format the data in a nicer fashion, and creates graphic visualizations
 by punching together various stretched single-pixel images */
class htmlReporter : public reporter {
	protected:
	QString links;
	QTextStream linksCollector;
	int targetcount;
	public:
	htmlReporter(const QString& title_ = "ESM Data");
	virtual QString str();
	virtual QString escapeString(const QString in) const;
	virtual void bigblockstart(const QString& name);
	virtual void bigblockend();
	virtual void blockstart(const QString& description, int blockcount);
	virtual void blockbreak();
	virtual void blockend();
	virtual void single(const QString& description, bool ishead = false);
	virtual void operator()(const QString& description, const QString& value);
	virtual void operator()(const QString& description, int value);
	virtual bool hasBarGraph() const;
};
#endif
