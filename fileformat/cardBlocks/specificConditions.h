#ifndef SPECIFICCONDITIONS_H
#define SPECIFICCONDITIONS_H

class specificConditions : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(specificConditions)
	struct Specific_Condition {
		Time time;
		int condition;
		Specific_Condition(Time time_, int condition_) :
			time(time_), condition(condition_) {
		}
		friend reporter& operator<<(reporter& report, Specific_Condition scond) {
			report(scond.time.str(), formatStrings::specificCondition(scond.condition));
			return report;
		}
	};
	public:
	typedef std::vector<Specific_Condition> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0522;
	QString name() const {
		return tr("Specific Conditions");
	}
	specificConditions(constDataPointer filewalker) :
		cardBlock(filewalker) 
	{
		for(constDataPointer i = start + 5; i < start + 5 + datasize; i += 5) {
			int time = BEInt32(i), cond = i[4];
			if(time != 0 || cond != 0) sub.push_back(Specific_Condition(time,
					cond));
		}
	}
	virtual void printOn(reporter& o) const {
		for(subiter i = sub.begin(); i < sub.end(); ++i)
			o << *i;
	}
};

#endif
