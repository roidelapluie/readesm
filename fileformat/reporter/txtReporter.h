#ifndef TXTREPORTER_H
#define TXTREPORTER_H

#include "reporter.h"

#include <QtCore/QString>

class txtReporter : public reporter {
	public:
	txtReporter(const QString& title_ = "ESM Data");
	virtual QString str();
	virtual void blockstart(const QString& description, int blockcount);
	virtual void blockend();
	virtual void blockbreak();
	virtual void single(const QString& description, bool ishead = false);
	virtual void operator()(const QString& description, const QString& value);
	virtual void operator()(const QString& description, int value);
	virtual void bigblockstart(const QString& name);
	virtual void bigblockend();

};

#endif
