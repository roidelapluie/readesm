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
			report("faultType", formatEventType(IntByte()));
			report("faultRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("faultBeginTime", readDate().str());
			report("faultEndTime", readDate().str());
			report("cardNumberDriverSlotBegin", fixedString(18));
			report("cardNumberCodriverSlotBegin",fixedString(18));
			report("cardNumberDriverSlotEnd",fixedString(18));
			report("cardNumberCodriverSlotEnd",fixedString(18));
		}
		for(reporter::subblock b = report.newsub("Events", IntByte()); b(); ++b){
			report("eventType", formatEventType(IntByte()));
			report("eventRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("eventBeginTime", readDate().str());
			report("eventEndTime", readDate().str());
			report("cardNumberDriverSlotBegin", fixedString(18));
			report("cardNumberCodriverSlotBegin",fixedString(18));
			report("cardNumberDriverSlotEnd",fixedString(18));
			report("cardNumberCodriverSlotEnd",fixedString(18));
			report("similarEventsNumber",IntByte());
		}
		report("lastOverspeedControlTime", readDate().str());
		report("firstOverspeedSince", readDate().str());
		report("numberOfOverspeedSince", IntByte());
		for(reporter::subblock b = report.newsub("ActivityChangeInfo", IntByte()); b(); ++b){
			report("eventType", formatEventType(IntByte()));
			report("eventRecordPurpose", formatEventRecordPurpose(IntByte()));
			report("eventBeginTime", readDate().str());
			report("eventEndTime",readDate().str());
			report("maxSpeedValue",IntByte());
			report("averageSpeedValue",IntByte());
			report("cardNumberDriverSlotBegin",fixedString(18));
			report("similarEventsNumber",IntByte());
		}
		for(reporter::subblock b = report.newsub("TimeAdjustments", IntByte()); b(); ++b){
			report("oldTimeValue",readDate().str());
			report("newTimeValue",readDate().str());
			report("workshopName",fixedString(36));
			report("workshopAddress",fixedString(36));
			report("workshopCardNumber",fixedString(18));
		}
	}
	void BriefReport(reporter& report) const{
		CompleteReport(report);
	}
};

#endif
