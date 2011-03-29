#ifndef DRIVERACTIVITYDATA_H
#define DRIVERACTIVITYDATA_H

#include "cardBlock.h"

#include "../Activity.h"
#include "../overtime.h"

///see p. 59
class driverActivityData : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(driverActivityData)
	public:
	static const int Type = 0x0504;
	QString name() const {
		return tr("Driver Activity Data");
	}
	Time LastCardDownload;
	int fine;
	int useddata;
	QByteArray actdata;
	driverActivityData(constDataPointer filewalker) :
		cardBlock(filewalker), 
		fine(0) 
	{
		int oldestrec = BEInt16(start + 5); //start of oldest rec
		int newestrec = BEInt16(start + 5 + 2); //end of newest rec (TODO:check this)
		if(newestrec < oldestrec) {
			//datasize is 2 byte for newest pointer, 2 byte for oldest pointer
			//plus activity data. so datasize - 4 is the activity data size
			//just copy the circular stuff into a new vector, avoids those boundary problems
			actdata.append(start.toPointer(9 + oldestrec), datasize - 4 - oldestrec);
			actdata.append(start.toPointer(9), newestrec);
		} else {
			actdata.append(start.toPointer(9 + oldestrec), newestrec - oldestrec);
		}
		constDataPointer walker(actdata,0);
		while(walker.bytesLeft()) {
			int thissize = BEInt16(walker + 2);
			//qDebug() << walker.bytesLeft() <<" Bytes left, " << thissize << "in next block";
			if(!thissize) {
				qDebug() << "Size 0 for increment. Aborting.";
				break;
			}
			dailyActivityCard d(walker, (thissize - 12) / 2);
			acts.push_back(d);
			walker += thissize;
		}
		fine = checkTimes(acts);
		qDebug() << "driverActivityData end";
	}
	typedef std::vector<dailyActivityCard> subray;
	typedef subray::const_iterator subiter;
	subray acts;

	virtual void printOn(reporter& o) const {
		o(tr("Accumulated fines"), QString::fromUtf8("%1 €").arg(fine));
		o(tr("Activity space usage"), tr("%1 of %2 Bytes")
						.arg(actdata.size())
						.arg(datasize - 9)
		 );
		o.reportraynosub(acts);

	}
	virtual void reportstuff(esmfilehead& esm) {
		for(subiter i = acts.begin(); i < acts.end(); ++i) {
			esm.reportDayStatistics(i->start, i->distance, i->driventime);
		}
	}

};

#endif