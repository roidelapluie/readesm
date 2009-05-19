/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef FAULTS_H
#define FAULTS_H FAULTS_H
#include "vublock.h"
#include "formatStrings.h"


class EventBase {
	public:
	static const int csize = 10;
	int Type, RecordPurpose;
	Time BeginTime, EndTime;
	EventBase(iter filewalker) :
		Type(filewalker[0]),
		RecordPurpose(filewalker[1]),
		BeginTime(BEInt32(filewalker + 2)),
		EndTime(BEInt32(filewalker + 6)){}
	virtual void printOn(reporter& o) const{
		o.single(formatEventType(Type), true);
		o.single(formatRange(BeginTime,EndTime));
		o("RecordPurpose", formatEventRecordPurpose(RecordPurpose));
	}
	friend reporter& operator<<(reporter& report, const EventBase& e){
		e.printOn(report);
		return report;
	}
};

class Fault : public EventBase {
	public:
	static const int csize = EventBase::csize + 4*18;
	string cardnumbers[4];
	static const int cardNumberDriverSlotBegin = 0;
	static const int cardNumberCodriverSlotBegin = 1;
	static const int cardNumberDriverSlotEnd = 2;
	static const int cardNumberCodriverSlotEnd = 3;
	Fault(iter filewalker) : EventBase(filewalker) {
		for(int j = 0; j < 4; ++j) cardnumbers[j] = fixedString(filewalker + EventBase::csize + j*18,18);
	}
	virtual void printOn(reporter& report) const{
		EventBase::printOn(report);
		if(cardnumbers[2] != "" && cardnumbers[2] != cardnumbers[0]){
			if(cardnumbers[0] != "") report("cardNumberDriverSlotBegin", cardnumbers[0]);
			if(cardnumbers[2] != "") report("cardNumberDriverSlotEnd", cardnumbers[2]);
		} else if(cardnumbers[0] != "") report("cardNumberDriverSlot", cardnumbers[0]);
		if(cardnumbers[3] != "" && cardnumbers[3] != cardnumbers[1]){
			if(cardnumbers[1] != "") report("cardNumberCodriverSlotBegin", cardnumbers[1]);
			if(cardnumbers[3] != "") report("cardNumberCodriverSlotEnd", cardnumbers[3]);
		} else if(cardnumbers[1] != "") report("cardNumberCodriverSlot", cardnumbers[1]);
	}
};

class Event : public Fault {
	public:
	static const int csize = Fault::csize + 1;
	int similarEventsNumber;
	Event(iter filewalker) : Fault(filewalker), similarEventsNumber(filewalker[Fault::csize]) {}
	virtual void printOn(reporter& report) const{
		Fault::printOn(report);
		if(similarEventsNumber) report("similarEventsNumber",similarEventsNumber);
	}
};

class Overspeed : public EventBase {
	public:
	static const int csize = EventBase::csize + 21;
	int maxSpeedValue, averageSpeedValue;
	string cardNumberDriverSlotBegin;
	int similarEventsNumber;
	Overspeed(iter filewalker) : EventBase(filewalker),
		maxSpeedValue(filewalker[10]),
		averageSpeedValue(filewalker[11]),
		cardNumberDriverSlotBegin(fixedString(filewalker + 12, 18)),
		similarEventsNumber(filewalker[30]) {}
	virtual void printOn(reporter& report) const{
		EventBase::printOn(report);
		report("maxSpeedValue",stringify(maxSpeedValue) + " km/h");
		report("averageSpeedValue",stringify(averageSpeedValue) + " km/h");
		report("cardNumberDriverSlotBegin",cardNumberDriverSlotBegin);
		if(similarEventsNumber) report("similarEventsNumber",similarEventsNumber);
	}
};


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
			Fault e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Fault::csize;
		}
		for(reporter::subblock b = report.newsub("Events", IntByte()); b(); ++b){
			Event e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Event::csize;
		}
		report("lastOverspeedControlTime", readDate().str());
		report("firstOverspeedSince", readDate().str());
		report("numberOfOverspeedSince", IntByte());
		for(reporter::subblock b = report.newsub("ActivityChangeInfo", IntByte()); b(); ++b){
			Overspeed e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Overspeed::csize;
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
