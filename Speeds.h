#ifndef SPEEDS_H
#define SPEEDS_H SPEEDS_H
#include <string>
#include "vublock.h"

///See page 163 of l207.pdf
class Speeds : public vublock {
	public:
	string name() const {
		return "Detailed Speed";
	}
	static const int TREP = 0x4;
	Speeds(iter nstart) : vublock(nstart) { Init(); }
	int size() const{
		return 2+64*Int16(0) + 2 + 128;;
	}
	void CompleteReport(reporter& report) const{
		runningIndex = 0;
		for(reporter::subblock b = report.newsub("Detailed Speed", Int16()); b(); ++b){
			report("BeginDate",readDate().str());
			ostringstream o;
			for(int k = 0; k < 60; ++k) o << IntByte() << ", ";
			report("Speeds",o.str());
		}
	}
	void BriefReport(reporter& report) const{
		if(!report.hasPlotGraph()){
			//this really requires a plotting program and stuff
			report("Speed data","omitted");
			return;
		}
		reporter::pgptr visual(report.getPlotGraph());
		runningIndex = 0;
		int count = Int16();
		if(!count) return;
		int offset = readDate().timestamp;
		for(int k = 0; k < 60; ++k) *visual << k << " " << IntByte() << "\n";
		for(int j = 1; j < count; ++j){
			int difference = readDate().timestamp - offset;
			for(int k = 0; k < 60; ++k) *visual << (difference + k) << " " << IntByte() << "\n";
		}
		report("Speeds",visual->str());
	}

};

#endif
