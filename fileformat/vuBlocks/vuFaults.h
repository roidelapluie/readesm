/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef FAULTS_H
#define FAULTS_H FAULTS_H

#include "../formatStrings.h"
#include "vuBlock.h"

class EventBase {
	Q_DECLARE_TR_FUNCTIONS(EventBase)

	public:
	static int csize() { return 10;}
	int Type, RecordPurpose;
	Time BeginTime, EndTime;
	EventBase(constDataPointer filewalker) :
		Type(filewalker[0]), RecordPurpose(filewalker[1]), BeginTime(BEInt32(
				filewalker + 2)), EndTime(BEInt32(filewalker + 6)) {
	}
	virtual void printOn(reporter& o) const {
		o.single(formatStrings::eventType(Type), true);
		o.single(formatRange(BeginTime, EndTime));
		o(tr("RecordPurpose"), formatStrings::eventRecordPurpose(RecordPurpose));
	}
	friend reporter& operator<<(reporter& report, const EventBase& e) {
		e.printOn(report);
		return report;
	}
};

class Fault : public EventBase {
	Q_DECLARE_TR_FUNCTIONS(Fault)
	public:
	static int csize() {return  EventBase::csize() + 4* 18; }
	QString cardnumbers[4];
	static const int cardNumberDriverSlotBegin = 0;
	static const int cardNumberCodriverSlotBegin = 1;
	static const int cardNumberDriverSlotEnd = 2;
	static const int cardNumberCodriverSlotEnd = 3;
	Fault(constDataPointer filewalker) :
		EventBase(filewalker) {
		for(int j = 0; j < 4; ++j)
			cardnumbers[j] = fixedString(
					filewalker + EventBase::csize() + j * 18, 18);
	}
	virtual void printOn(reporter& report) const {
		EventBase::printOn(report);
		if(cardnumbers[2] != "" && cardnumbers[2] != cardnumbers[0]) {
			if(cardnumbers[0] != "") report(tr("cardNumberDriverSlotBegin"),
					cardnumbers[0]);
			if(cardnumbers[2] != "") report(tr("cardNumberDriverSlotEnd"),
					cardnumbers[2]);
		} else if(cardnumbers[0] != "") report(tr("cardNumberDriverSlot"),
				cardnumbers[0]);
		if(cardnumbers[3] != "" && cardnumbers[3] != cardnumbers[1]) {
			if(cardnumbers[1] != "") report(tr("cardNumberCodriverSlotBegin"),
					cardnumbers[1]);
			if(cardnumbers[3] != "") report(tr("cardNumberCodriverSlotEnd"),
					cardnumbers[3]);
		} else if(cardnumbers[1] != "") report(tr("cardNumberCodriverSlot"),
				cardnumbers[1]);
	}
};

class Event : public Fault {
	Q_DECLARE_TR_FUNCTIONS(Event);
	public:
	static int csize() { return Fault::csize() + 1; }
	int similarEventsNumber;
	Event(constDataPointer filewalker) :
		Fault(filewalker), similarEventsNumber(filewalker[Fault::csize()]) {
	}
	virtual void printOn(reporter& report) const {
		Fault::printOn(report);
		if(similarEventsNumber) report(tr("similarEventsNumber"),
				similarEventsNumber);
	}
};

class Overspeed : public EventBase {
	Q_DECLARE_TR_FUNCTIONS(Overspeed);
	public:
	static int csize() { return EventBase::csize() + 21; }
	int maxSpeedValue, averageSpeedValue;
	QString cardNumberDriverSlotBegin;
	int similarEventsNumber;
	Overspeed(constDataPointer filewalker) :
		EventBase(filewalker), maxSpeedValue(filewalker[10]),
				averageSpeedValue(filewalker[11]), cardNumberDriverSlotBegin(
						fixedString(filewalker + 12, 18)), similarEventsNumber(
						filewalker[30]) {
	}
	virtual void printOn(reporter& report) const {
		EventBase::printOn(report);
		report(tr("maxSpeedValue"), stringify(maxSpeedValue) + " km/h");
		report(tr("averageSpeedValue"), stringify(averageSpeedValue) + " km/h");
		report(tr("cardNumberDriverSlotBegin"), cardNumberDriverSlotBegin);
		if(similarEventsNumber) report(tr("similarEventsNumber"),
				similarEventsNumber);
	}
};

///See page 162 of l207.pdf
class vuFaults : public vuBlock {
	Q_DECLARE_TR_FUNCTIONS(vuFaults)
	public:
	QString name() const {
		return tr("Faults");
	}
	static const int TREP = 0x3;
	vuFaults(constDataPointer nstart) :
		vuBlock(nstart) {
		Init();
	}
	int size() const {
		int index = 82* IntByte (0) + 1;
		index += 83* IntByte (index) + 1 + 9;
		index += 31* IntByte (index) + 1;
		index += 98* IntByte (index) + 1;
		return 2 + index + 128;
	}
	void CompleteReport(reporter& report) const {
		runningIndex = 0;
		for(reporter::subblock b = report.newsub(tr("Faults"), IntByte()); b(); ++b) {
			Fault e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Fault::csize();
		}
		for(reporter::subblock b = report.newsub(tr("Events"), IntByte()); b(); ++b) {
			Event e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Event::csize();
		}
		report(tr("lastOverspeedControlTime"), readDate().str());
		report(tr("firstOverspeedSince"), readDate().str());
		report(tr("numberOfOverspeedSince"), IntByte());
		for(reporter::subblock b = report.newsub(tr("ActivityChangeInfo"),
				IntByte()); b(); ++b) {
			Overspeed e(start + runningIndex + 2);
			e.printOn(report);
			runningIndex += Overspeed::csize();
		}
		for(reporter::subblock b = report.newsub(tr("TimeAdjustments"), IntByte()); b(); ++b) {
			report(tr("oldTimeValue"), readDate().str());
			report(tr("newTimeValue"), readDate().str());
			report(tr("workshopName"), fixedString(36));
			report(tr("workshopAddress"), fixedString(36));
			report(tr("workshopCardNumber"), fixedString(18));
		}
	}
	void BriefReport(reporter& report) const {
		CompleteReport(report);
	}
};

#endif
