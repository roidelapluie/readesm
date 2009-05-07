/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef FAULTS_H
#define FAULTS_H FAULTS_H
#include "vublock.h"
#include "formatStrings.h"

///See page 162 of l207.pdf
class Faults : public vublock {
	public:
	string name() const {
		return "Faults";
	}
	static const int TREP = 0x3;
	Faults(iter nstart) : vublock(nstart) { Init(); }
	int size() const{
		int index = 82*IntByte(0) + 1;
		index += 83*IntByte(index) + 1 + 9;
		index += 31*IntByte(index) + 1;
		index += 98*IntByte(index) + 1;
		return 2 + index + 128;
	}
	void CompleteReport(reporter& report) const{
		runningIndex = 0;
		for(reporter::subblock b = report.newsub("Faults", IntByte()); b(); ++b){
			report("FaultType", formatEventType(IntByte()));
			report("FaultRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("FaultBeginTime", readDate().str());
			report("FaultEndTime", readDate().str());
			report("CardNumberDriverSlotBegin", fixedString(18));
			report("CardNumberCodriverSlotBegin",fixedString(18));
			report("CardNumberDriverSlotEnd",fixedString(18));
			report("CardNumberCodriverSlotEnd",fixedString(18));
		}
		for(reporter::subblock b = report.newsub("Events", IntByte()); b(); ++b){
			report("EventType", formatEventType(IntByte()));
			report("EventRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("EventBeginTime", readDate().str());
			report("EventEndTime", readDate().str());
			report("CardNumberDriverSlotBegin", fixedString(18));
			report("CardNumberCodriverSlotBegin",fixedString(18));
			report("CardNumberDriverSlotEnd",fixedString(18));
			report("CardNumberCodriverSlotEnd",fixedString(18));
			report("SimilarEventsNumber",IntByte());
		}
		report("LastOverspeedControlTime", readDate().str());
		report("FirstOverspeedSince", readDate().str());
		report("NumberOfOverspeedSince", IntByte());
		for(reporter::subblock b = report.newsub("ActivityChangeInfo", IntByte()); b(); ++b){
			report("EventType", formatEventType(IntByte()));
			report("EventRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("EventBeginTime", readDate().str());
			report("EventEndTime",readDate().str());
			report("MaxSpeedValue",IntByte());
			report("AverageSpeedValue",IntByte());
			report("CardNumberDriverSlotBegin",fixedString(18));
			report("SimilarEventsNumber",IntByte());
		}
		for(reporter::subblock b = report.newsub("TimeAdjustments", IntByte()); b(); ++b){
			report("OldTimeValue",readDate().str());
			report("NewTimeValue",readDate().str());
			report("WorkshopName",fixedString(36));
			report("WorkshopAddress",fixedString(36));
			report("WorkshopCardNumber",fixedString(18));
		}
	}
	void BriefReport(reporter& report) const{
		CompleteReport(report);
	}
};

#endif
