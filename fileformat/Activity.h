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

#ifndef ACTIVITY_H
#define ACTIVITY_H
#include <QString>
#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include "reporter.h"
#include "myTime.h"
#include "readTypes.h"
#include "helper.h"

class Activity {
	Q_DECLARE_TR_FUNCTIONS(Activity)
	public:
	static const int Break = 0;
	static const int Available = 1;
	static const int Work = 2;
	static const int Driving = 3;
	static const int Driver = 0;
	static const int Codriver = 1;
	static const int Crew = 1;

	static QString formatDurTime(int offset) {
		return QString("%1:%2")
				.arg(offset / 60, 1, 10, QChar('0'))
				.arg(offset % 60, 2, 10, QChar('0'));
	}

	Activity() :
		slot(0), manning(0), cardin(0), activity(0), time(0), duration(0) {
	}
	Activity(constDataPointer start) {
		slot = (start[0] & (1 << 7)) >> 7;
		manning = (start[0] & (1 << 6)) >> 6;
		cardin = (start[0] & (1 << 5)) >> 5;
		activity = (start[0] & ((1 << 4) | (1 << 3))) >> 3;
		time = int((start[0] & 7) << 8) + start[1];
	}
	Activity(const unsigned char* start) {
		slot = (start[0] & (1 << 7)) >> 7;
		manning = (start[0] & (1 << 6)) >> 6;
		cardin = (start[0] & (1 << 5)) >> 5;
		activity = (start[0] & ((1 << 4) | (1 << 3))) >> 3;
		time = int((start[0] & 7) << 8) + start[1];
	}
	QString astr() const {
		if(activity == Break) return tr("break/rest");
		else if(activity == Available) return tr("availability");
		else if(activity == Work) return tr("work");
		else if(activity == Driving) return tr("driving");
		return tr("unknown activity");
	}
	QString tstr(int offset = 0) const {
		return formatDurTime(time + offset);
	}
	QString str() const {
		QString rv;
		QTextStream o(&rv);
		o << (slot == Codriver ? tr("co-driver") : tr("driver")) << ", ";
		o << (manning == Crew ? tr("crew") : tr("single")) << ", ";
		o << "card " << (cardin ? tr("not inserted") : tr("inserted")) << ", ";
		o << astr() << " " << tstr();
		return rv;
	}
	int slot, manning, cardin, activity, time, duration;
	friend QTextStream& operator<<(QTextStream& o, const Activity& a) {
		o << a.str();
		return o;
	}
	friend reporter& operator<<(reporter& o, const Activity& a) {
		o(a.tstr(), a.str());
		return o;
	}
};

QString visualization(reporter& o, const std::vector<Activity>& acts); 

class dailyActivity {
	Q_DECLARE_TR_FUNCTIONS(dailyActivity)
	public:
	Time start;
	typedef std::vector<Activity> subray;
	typedef subray::const_iterator subiter;
	subray driver;
	subray codriver;
	int driventime;
	int overtime;
	QString overtimeReason;
	QString weekStats;
	int fine;
	void calcDurations(std::vector<Activity>& acts) {
		if(!acts.size()) return;
		for(unsigned int j = 1; j < acts.size(); ++j) {
			acts[j - 1].duration = acts[j].time - acts[j - 1].time;
			if(acts[j - 1].duration < 0) {
				qDebug() << "duration < 0\n";
			}
			if(acts[j - 1].activity == Activity::Driving) {
				driventime += acts[j - 1].duration;
			}
		}
		acts[acts.size() - 1].duration = 24* 60 - acts [acts.size() - 1].time;
	}
	dailyActivity(constDataPointer datepos, constDataPointer data, int count) :
		start(BEInt32(datepos)), driventime(0), overtime(0) {
		for(int j = 0; j < count; ++j) {
			Activity Act(data + 2 * j);
			if(Act.slot == Activity::Driver) driver.push_back(Act);
			else if(Act.slot == Activity::Codriver) codriver.push_back(Act);
		}
		calcDurations(driver);
		calcDurations(codriver);
	}
	dailyActivity() {
	}
	friend reporter& operator<<(reporter& o, const dailyActivity& d) {
		if(d.driventime) {
			o(tr("Date"), d.start.datestr());
			if(d.driver.size() >= 2 - (unsigned int) o.verbose) {
				if(o.verbose) o.reportray(d.driver, tr("Daily Activity Driver"));
				if(o.hasBarGraph()) o.single(tr("Activities Driver") + "<br />" + visualization(o,
						d.driver));
			}
			if(d.codriver.size() >= 2 - (unsigned int) o.verbose) {
				if(o.verbose) o.reportray(d.codriver, tr("Daily Activity Codriver"));
				if(o.hasBarGraph()) o.single(tr("Activities Codriver") + "<br />" + visualization(o,
						d.driver));
			}
			o(tr("Driving time"), Activity::formatDurTime(d.driventime));
			if(d.overtime) {
				o(tr("Overtime"), Activity::formatDurTime(d.overtime) + " "
						+ d.overtimeReason);
				o(tr("Possible fine"), stringify(d.fine) + QString::fromUtf8(" €"));
			}
		}
		if(d.weekStats != "") {
			o(tr("Week"), d.weekStats);
		}
		return o;
	}
};

///p. 57 and corrigendum(!)
class dailyActivityCard : public dailyActivity {
	Q_DECLARE_TR_FUNCTIONS(dailyActivityCard)
	public:
	int presence, distance;
	dailyActivityCard(constDataPointer start, int count) :
		dailyActivity(start + 4, start + 12, count), 
		presence(BEInt16(start + 8)), 
		distance(BEInt16(start + 10)) 
	{
	}
	friend reporter& operator<<(reporter& o, const dailyActivityCard& d) {
		o << (dailyActivity) d;
		if(o.verbose) o(tr("activityDailyPresenceCounter"), d.presence);
		if(d.distance) o(tr("activityDayDistance"), stringify(d.distance) + " km");
		return o;
	}
};
#endif
