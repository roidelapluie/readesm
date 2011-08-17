#include "VuDetailedSpeedData.h"

VuDetailedSpeedData::VuDetailedSpeedData(const constDataPointer& start) : RawVuDetailedSpeedData(start) {}

void VuDetailedSpeedData::printOn(reporter& report) const {
	if(!report.hasPlotGraph()) {
		//this really requires a plotting program and stuff
		report(tr("Speed data"), tr("omitted"));
		return;
	}
	if(vuDetailedSpeedBlocks.size() < 1) return;
	
	reporter::pgptr visual(report.getPlotGraph());
	QDateTime daystart = QDateTime(vuDetailedSpeedBlocks[0].speedBlockBeginDate.date());
	int lastDifference = 0;
	for(uint j = 0; j < vuDetailedSpeedBlocks.size(); ++j) {
		int difference = daystart.secsTo(vuDetailedSpeedBlocks[j].speedBlockBeginDate);
		if(difference > 86400){
			visual->collector << (lastDifference + 1) << " 0\n";
			report(daystart.date().toString(), visual->str());
			daystart = QDateTime(vuDetailedSpeedBlocks[j].speedBlockBeginDate.date());
			difference = daystart.secsTo(vuDetailedSpeedBlocks[j].speedBlockBeginDate);
			visual->reset();
			lastDifference = 0;
		}
		if(difference > lastDifference + 60){
			visual->collector << (lastDifference + 1) << " 0\n";
			visual->collector << (difference - 1) << " 0\n";
		}
		for(int k = 0; k < 60; ++k)
			visual->collector << (difference + k) << " " << vuDetailedSpeedBlocks[j].speedsPerSecond[k] << "\n";
		lastDifference = difference;
	}
	if(lastDifference != 0){
		visual->collector << (lastDifference + 1) << " 0\n";
		report(daystart.date().toString(), visual->str());
	}
		
}