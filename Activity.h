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
#include <string>
#include <sstream>
#include <iomanip>
#include "reporter.h"
#include "typedefs.h"
#include "time.h"
#include "readTypes.h"

class Activity {
	public:
	static const int Break = 0;
	static const int Available = 1;
	static const int Work = 2;
	static const int Driving = 3;
	static const int Driver = 0;
	static const int Codriver = 1;
	static const int Crew = 1;
	static string formatDurTime(int offset = 0) {
		ostringstream o;
		o << std::setw(2) << std::setfill('0') << ((offset) / 60) << ":"
				<< std::setw(2) << std::setfill('0') << ((offset) % 60);
		return o.str();
	}

	Activity() :
		slot(0), manning(0), cardin(0), activity(0), time(0), duration(0) {
	}
	Activity(iter start) {
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
	string astr() const {
		if(activity == Break) return "break/rest";
		else if(activity == Available) return "availability";
		else if(activity == Work) return "work";
		else if(activity == Driving) return "driving";
		return "unknown activity";
	}
	string tstr(int offset = 0) const {
		return formatDurTime(time + offset);
	}
	string str() const {
		ostringstream o;
		o << (slot == Codriver ? "co-driver" : "driver") << ", ";
		o << (manning == Crew ? "crew" : "single") << ", ";
		o << "card " << (cardin ? "not inserted" : "inserted") << ", ";
		o << astr() << " " << tstr();
		return o.str();
	}
	int slot, manning, cardin, activity, time, duration;
	friend std::ostream& operator<<(std::ostream& o, const Activity& a) {
		o << a.str();
		return o;
	}
	friend reporter& operator<<(reporter& o, const Activity& a) {
		o(a.tstr(), a.str());
		return o;
	}
};

string visualization(reporter& o, const std::vector<Activity>& acts) {
	reporter::pgptr actvisual(o.getBarGraph());
	for(std::vector<Activity>::const_iterator j(acts.begin()); j != acts.end(); ++j) {
		if(j->duration > 10000) std::cerr << "ouch";
		string descr = tr(j->astr()) + " " + tr("for") + " "
				+ Activity::formatDurTime(j->duration) + " " + tr("from") + " "
				+ tr(j->tstr()) + " " + tr("to") + " " + j->tstr(j->duration);
		int height = 10;
		string color;
		switch(j->activity) {
			case Activity::Work:
				height = 70;
				color = "yellow";
				break;
			case Activity::Available:
				height = 10;
				color = "black";
				break;
			case Activity::Driving:
				height = 100;
				color = "green";
				break;
			case Activity::Break:
				if(j->duration >= 15) {
					height = 40;
					color = "red";
				} else {
					height = 50;
					color = "blue";
				}
				break;
			default:
				*actvisual << (j->activity) << (j->str()) << descr;
		}
		actvisual->add(j->time, j->duration, height, color, descr);
	}
	return actvisual->str();
}

class DailyActivity {
	public:
	Time start;
	typedef std::vector<Activity> subray;
	typedef subray::const_iterator subiter;
	subray driver;
	subray codriver;
	int driventime;
	int overtime;
	string overtimeReason;
	string weekStats;
	int fine;
	void calcDurations(std::vector<Activity>& acts) {
		if(!acts.size()) return;
		for(unsigned int j = 1; j < acts.size(); ++j) {
			acts[j - 1].duration = acts[j].time - acts[j - 1].time;
			if(acts[j - 1].duration < 0) {
				std::cerr << tr("duration < 0\n");
			}
			if(acts[j - 1].activity == Activity::Driving) {
				driventime += acts[j - 1].duration;
			}
		}
		acts[acts.size() - 1].duration = 24* 60 - acts [acts.size() - 1].time;
	}
	DailyActivity(iter datepos, iter data, int count) :
		start(BEInt32(datepos)), driventime(0), overtime(0) {
		for(int j = 0; j < count; ++j) {
			Activity Act(data + 2* j );
			if(Act.slot == Activity::Driver) driver.push_back(Act);
			else if(Act.slot == Activity::Codriver) codriver.push_back(Act);
		}
		calcDurations(driver);
		calcDurations(codriver);
	}
	DailyActivity() {
	}
	friend reporter& operator<<(reporter& o, const DailyActivity& d) {
		if(d.driventime) {
			o("Date", d.start.datestr());
			if(d.driver.size() >= 2 - (unsigned int) o.verbose) {
				if(o.verbose) o.reportray(d.driver, "Daily Activity Driver");
				if(o.hasBarGraph()) o("Activities Driver", visualization(o,
						d.driver));
			}
			if(d.codriver.size() >= 2 - (unsigned int) o.verbose) {
				if(o.verbose) o.reportray(d.codriver, "Daily Activity Codriver");
				if(o.hasBarGraph()) o("Activities Codriver", visualization(o,
						d.codriver));
			}
			o("Driving time", Activity::formatDurTime(d.driventime));
			if(d.overtime) {
				o("Overtime", Activity::formatDurTime(d.overtime) + " "
						+ d.overtimeReason);
				o("Possible fine", stringify(d.fine) + " €");
			}
		}
		if(d.weekStats != "") {
			o("Week", d.weekStats);
		}
		return o;
	}
};

///p. 57 and corrigendum(!)
class DailyActivityCard : public DailyActivity {
	public:
	int presence, distance;
	DailyActivityCard(iter start, int count) :
		DailyActivity(start + 4, start + 12, count), presence(
				BEInt16(start + 8)), distance(BEInt16(start + 10)) {
	}
	friend reporter& operator<<(reporter& o, const DailyActivityCard& d) {
		o << (DailyActivity) d;
		if(o.verbose) o("activityDailyPresenceCounter", d.presence);
		if(d.distance) o("activityDayDistance", stringify(d.distance) + " km");
		return o;
	}
};
#endif
