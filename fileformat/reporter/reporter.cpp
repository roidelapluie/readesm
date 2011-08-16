#include "reporter.h"

reporter::reporter(const QString& title_) :
	collected(),
	collector(&collected),
	title(title_),
	verbose(false)
{
}

reporter::subblock reporter::newsub(const QString& description, int blockcount) {
	return subblock(description, blockcount, this);
}

QString reporter::str() {
	return collected;
}

void reporter::bigblockbreak() {
	blockbreak();
}

bool reporter::hasBarGraph() const {
	return false;
}

reporter::pgptr reporter::getBarGraph() const {
	return pgptr(new picgen);
}

bool reporter::hasPlotGraph() const {
	return false;
}

reporter::pgptr reporter::getPlotGraph() const {
	return pgptr(new picgen);
}

QString reporter::InfoAppend(bool html, bool justname) {
	QString rv;
	if(!justname) rv += tr("Report generated with") + " ";
	if(html) rv += "<a href='http://readesm.sourceforge.net/'>readESM</a>";
	else rv += "readESM (http://readesm.sourceforge.net/)";
#ifdef VERSION
	rv += " " + tr("version") + " " VERSION;
#endif
	return rv + "\n";
}
