#ifndef VEHICLESUSED_H
#define VEHICLESUSED_H

class vehiclesUsed : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(vehiclesUsed)
	struct CardVehicleRecord {
		int OdometerBegin, OdometerEnd;
		Time FirstUse, LastUse;
		vehicleRegistration vreg;
		int vuDataBlockCounter;
		CardVehicleRecord(constDataPointer start) :
			OdometerBegin(BEInt24(start)), 
			OdometerEnd(BEInt24(start + 3)),
			FirstUse(BEInt32(start + 6)), 
			LastUse(BEInt32(start + 10)),
			vreg(start + 14), 
			vuDataBlockCounter(BEInt16(start + 29)) 
		{
		}
		friend reporter& operator<<(reporter& report,
				const CardVehicleRecord& e) {
			report << e.vreg;
			report(tr("vehicleFirstUse"), e.FirstUse.str());
			report(tr("vehicleLastUse"), e.LastUse.str());
			report(tr("vehicleOdometerBegin"), e.OdometerBegin);
			report(tr("vehicleOdometerEnd"), e.OdometerEnd);
			report(tr("vuDataBlockCounter"), e.vuDataBlockCounter);
			return report;
		}
		static bool defval(constDataPointer start) {
			return checkZeros(start, 14) && vehicleRegistration::defval(start
					+ 14) && checkZeros(start + 29, 2);
		}
	};
	public:
	typedef std::vector<CardVehicleRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0505;
	virtual QString name() const {
		return tr("Vehicles Used");
	}
	vehiclesUsed(constDataPointer filewalker) :
		cardBlock(filewalker) 
	{
		for(constDataPointer i = start + 5 + 2; i < start + 5 + datasize; i += 31) {
			if(!CardVehicleRecord::defval(i)) sub.push_back(
					CardVehicleRecord(i));
		}
	}
	virtual void printOn(reporter& report) const {
		if(report.verbose) report.reportraynosub(sub);
		else {
			for(subiter i = sub.begin(); i < sub.end(); ++i) {
				subiter lastentry = i;
				while(i < (sub.end() - 1) && i->LastUse.timestamp % 86400 == 86399)
					++i;
				report << i->vreg;
				report.single(formatRange(lastentry->FirstUse, i->LastUse));
				report(tr("vehicleOdometerBegin"), lastentry->OdometerBegin);
				report(tr("vehicleOdometerEnd"), i->OdometerEnd);
				int driven = i->OdometerEnd - lastentry->OdometerBegin;
				report(tr("Driven distance"), QString("%1 km").arg(driven));
				if(i < sub.end()) report.blockbreak();
			}
		}
	}
};

#endif
