#ifndef XMLREPORTER_H
#define XMLREPORTER_H

#include "htmlReporter.h"

#include <QtCore/QString>

///The XHTML(inline svg graphics) reporter class
/** This reporter aims to produce one single file of output with everything embedded within it.
 To do that it embeds svg into the html, it works in firefox, not in konqueror :( */
class xmlReporter : public htmlReporter {
	public:
	virtual QString str();
	xmlReporter(const QString& title_ = "ESM Data");
	virtual bool hasPlotGraph() const;
};


#endif
