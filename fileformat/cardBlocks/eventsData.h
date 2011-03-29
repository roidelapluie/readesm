#ifndef EVENTSDATA_H
#define EVENTSDATA_H

class eventsData : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(eventsData)
	struct CardEventRecord {
		int Type;
		Time BeginTime, EndTime;
		vehicleRegistration vreg;
		CardEventRecord(constDataPointer start) :
			Type(start[0]), 
			BeginTime(BEInt32(start + 1)), 
			EndTime(BEInt32(start + 5)), 
			vreg(start + 9) 
		{
		}
		friend reporter& operator<<(reporter& report, CardEventRecord e) {
			report(tr("eventType"), formatStrings::eventType(e.Type));
			report(tr("beginTime"), e.BeginTime.str());
			report(tr("endTime"), e.EndTime.str());
			report << e.vreg;
			return report;
		}
		static bool defval(constDataPointer start) {
			return checkZeros(start, 9) && vehicleRegistration::defval(start
					+ 9);
		}

	};
	public:
	typedef std::vector<CardEventRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0502;
	virtual QString name() const {
		return tr("Events Data");
	}
	eventsData(constDataPointer filewalker) :
		cardBlock(filewalker) {
		for(constDataPointer i = start + 5; i < start + 5 + datasize; i += 24) {
			if(!CardEventRecord::defval(i)) sub.push_back(CardEventRecord(i));
		}
	}
	virtual void printOn(reporter& o) const {
		for(subiter i = sub.begin(); i < sub.end(); ++i)
			o << *i;
	}
};

#endif
