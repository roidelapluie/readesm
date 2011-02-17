#include "legacyWrapper.h"

#include "fileformat/i18n.h"
#include "fileformat/esmfile.h"
#include "fileformat/reporter.h"

void initTranslation(){
	i18nInit();
}

std::string convertFile(const std::string& filename){
	esmfile f(filename);
	xmlreporter report;
	report << f;
	slurptofile("last.xml", report.str());
	return report.str();
}

