#include "CardDriverActivity.h"

#include <QtCore/QDebug>

template<typename T, int n>
class miniVector{
protected:
	QVector<T> content;
public:
	miniVector() : content(n){}
	T operator[](int j) const { return content[j]; }
	T& operator[](int j) { return content[j]; }
	miniVector& operator+=(const miniVector& other) {
		for(int j = 0; j < n; ++j) content[j] += other.content[j];
		return *this;
	}
};

typedef miniVector<int,6> activityVector;

QByteArray CardDriverActivity::readCyclicData(){
	QByteArray rv;
	if(newestRecord < oldestRecord) {
		//just copy the circular stuff into a new vector, avoids those boundary problems
		rv.append(cyclicData.toPointer(oldestRecord), cyclicData.size() - oldestRecord);
		rv.append(cyclicData.toPointer(), newestRecord);
	} else {
		rv.append(cyclicData.toPointer(oldestRecord), newestRecord - oldestRecord);
	}
	return rv;
}
CardDriverActivity::CardDriverActivity(const DataPointer& start) : RawCardDriverActivity(start),
	activityDataUncycled(readCyclicData()),
	cardActivityDailyRecords(Subblocks<CardActivityDailyRecord>::fromTypeAndLength(DataPointer(activityDataUncycled), activityDataUncycled.size()))
{
}

void CardDriverActivity::printOn(Reporter& report) const{
	report.tagValuePair(tr("oldestRecord"), oldestRecord);
	report.tagValuePair(tr("newestRecord"), newestRecord);
	report.writeArray(cardActivityDailyRecords, tr("cardActivityDailyRecords"));
	if(cardActivityDailyRecords.numberOfBlocks() > 0){
		QString tablehead = "<table><tr><th></th><th>Driving</th><th>Working</th><th>Rest</th><th>Available</th></tr>";
		activityVector durationsMonth;
		int currentMonth = cardActivityDailyRecords[0].activityRecordDate.date().month();
		QString table = QString("<ul><li>%1:").arg(tr("Timesheet for %1").arg(QDate::longMonthName(currentMonth))) + tablehead;
		for(int j = 0; j < cardActivityDailyRecords.numberOfBlocks(); ++j){
			if(cardActivityDailyRecords[j].activityRecordDate.date().month() != currentMonth){
				currentMonth = cardActivityDailyRecords[j].activityRecordDate.date().month();
				table += QString("<tr><th>Summary</th><th>%1</th><th>%2</th><th>%3</th><th>%4</th></tr>").arg(ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::DRIVING]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::WORK]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::REST] + durationsMonth[ActivityChangeInfo::SHORTREST]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::AVAILABLE]));

				durationsMonth = activityVector();
				table += QString("</table></li><li>%1:").arg(tr("Timesheet for %1").arg(QDate::longMonthName(currentMonth))) + tablehead;
			}
			activityVector durations;
			for(int k = 0; k < cardActivityDailyRecords[j].activityChangeInfos.numberOfBlocks(); ++k){
				durations[cardActivityDailyRecords[j].activityChangeInfos[k].activity] += cardActivityDailyRecords[j].activityChangeInfos[k].duration;
			}
			table += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>").arg( cardActivityDailyRecords[j].activityRecordDate.toString(), ActivityChangeInfo::formatClock(durations[ActivityChangeInfo::DRIVING]), ActivityChangeInfo::formatClock(durations[ActivityChangeInfo::WORK]), ActivityChangeInfo::formatClock(durations[ActivityChangeInfo::REST] + durations[ActivityChangeInfo::SHORTREST]), ActivityChangeInfo::formatClock(durations[ActivityChangeInfo::AVAILABLE]));
			durationsMonth += durations;
		}
		table += QString("<tr><th>Summary</th><th>%1</th><th>%2</th><th>%3</th><th>%4</th></tr>").arg(ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::DRIVING]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::WORK]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::REST] + durationsMonth[ActivityChangeInfo::SHORTREST]), ActivityChangeInfo::formatClock(durationsMonth[ActivityChangeInfo::AVAILABLE]));

		table += "</table></li></ul>";
		report.tagValuePair(tr("Timesheet"), table);
	}
}
