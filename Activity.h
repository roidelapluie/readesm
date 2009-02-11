/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef ACTIVITY_H
#define ACTIVITY_H
#include <string>
#include <sstream>
#include <iomanip>
#include "reporter.h"
#include "typedefs.h"
#include "time.h"
#include "readTypes.h"

class Activity{
	public:
	static const int Break = 0;
	static const int Available = 1;
	static const int Work = 2;
	static const int Driving = 3;
	static const int Driver = 0;
	static const int Codriver = 1;
	static const int Crew = 1;
	static std::string formatDurTime(int offset = 0){
		std::ostringstream o;
		o << std::setw(2) << std::setfill('0') << ((offset) / 60) << ":" << std::setw(2) << std::setfill('0') << ((offset) % 60);
		return o.str();
	}

	Activity() : slot(0), manning(0), cardin(0), activity(0), time(0), duration(0) {}
	Activity(iter start){
		slot     = (start[0] & (1<<7)) >> 7;
		manning  = (start[0] & (1<<6)) >> 6;
		cardin   = (start[0] & (1<<5)) >> 5;
		activity = (start[0] & ((1 << 4) | (1 << 3))) >> 3;
		time = int((start[0] & 7) << 8) + start[1];
	}
	Activity(const unsigned char* start){
		slot     = (start[0] & (1<<7)) >> 7;
		manning  = (start[0] & (1<<6)) >> 6;
		cardin   = (start[0] & (1<<5)) >> 5;
		activity = (start[0] & ((1 << 4) | (1 << 3))) >> 3;
		time = int((start[0] & 7) << 8) + start[1];
	}
	std::string astr() const{
		if(activity == Break) return "Break/rest";
		else if(activity == Available) return "Available";
		else if(activity == Work) return "Work";
		else if(activity == Driving) return "Driving";
		return "Unknown Activity";
	}
	std::string tstr(int offset = 0) const{
		return formatDurTime(time + offset);
	}
	std::string str() const{
		std::ostringstream o;
		o << (slot == Codriver ? "Codriver" : "Driver") << ", ";
		o << (manning == Crew ? "Crew  " : "Single") << ", ";
		o << "Card " << (cardin ? "not" : "") << " inserted, ";
		o << astr() << " " << tstr();
		return o.str();
	}
	int slot, manning, cardin, activity, time, duration;		
	friend std::ostream& operator<<(std::ostream& o, const Activity& a){ 
		o << a.str();
		return o;
	}
	friend reporter& operator<<(reporter& o, const Activity& a){ 
		o(a.tstr(), a.str());
		return o;
	}
};


void image(std::ostream& o, const std::string& filename, int height, int width, const std::string& title){
	o << "<img src='images/" << filename <<"' width='" << width << "' height='" << height << "' title='" << title << "' alt='" << title << "'/>";
}


std::string visualization(const std::vector<Activity>& acts){
	std::ostringstream actvisual;
	for(std::vector<Activity>::const_iterator j(acts.begin()); j != acts.end(); ++j){
		if(j->duration > 10000) std::cerr << "ouch";
		std::string descr = j->astr() + " for " + Activity::formatDurTime(j->duration) + "  from " + j->tstr() + " to " + j->tstr(j->duration);
		int act = j->activity;
		if(act == Activity::Work) image(actvisual,"yellow.gif",70,j->duration / 2,descr);
		else if(act == Activity::Available)  image(actvisual,"black.gif",10,j->duration / 2,descr);
		else if(act == Activity::Driving) image(actvisual,"green.gif",100,j->duration / 2,descr);
		else if(act == Activity::Break){
			if(j->duration >= 15)
			image(actvisual,"red.gif",40,j->duration / 2,descr);
			else image(actvisual,"blue.gif",50,j->duration / 2,descr);
		}
		else{
			actvisual << act << j->str();
		}
	}
	actvisual << "<br/>";
	image(actvisual,"scale.gif",20,1440 / 2,"scale");
	return actvisual.str();
}

class DailyActivity{
	public:
	Time start;
	typedef std::vector<Activity> subray;
	typedef subray::const_iterator subiter;
	subray driver;
	subray codriver;
	int driventime;
	int overtime;
	void calcDurations(std::vector<Activity>& acts) const{
		if(!acts.size()) return;
		for(unsigned int j = 1; j < acts.size(); ++j){
			acts[j - 1].duration = acts[j].time - acts[j-1].time;
			if(acts[j - 1].duration < 0){
//				std::cerr << "duration < 0\n";
			}
		}
		acts[acts.size() -1].duration = 24*60 - acts[acts.size() -1].time;
	}
	DailyActivity(iter datepos, iter data, int count) : start(BEInt32(datepos)), driventime(0), overtime(0) {
		for(int j = 0; j < count; ++j){
			Activity Act(data + 2*j);
			if(Act.slot == Activity::Driver) driver.push_back(Act);
			else if(Act.slot == Activity::Codriver) codriver.push_back(Act);
		}
		calcDurations(driver);
		calcDurations(codriver);
		int sincelastbreak = 0;
		bool had15minbreak = false;
		for(subiter i = driver.begin(); i < driver.end(); ++i){
			const int& d = i->duration;
			if(i->activity == Activity::Driving){
				driventime += d;
				sincelastbreak += d;
			}
			if(i->activity == Activity::Break && d >= 15){	
				if(d < 30) had15minbreak = true;
				else if(d >= 30){
					if(had15minbreak || d >= 45){ 
						if(sincelastbreak > 270) overtime += sincelastbreak - 270;
						sincelastbreak = 0;
						had15minbreak = false;
					}
					else had15minbreak = true;
				} 
			}
		}
		if(sincelastbreak > 270) overtime += sincelastbreak - 270;
		overtime = std::max(overtime, driventime - 600);
	}
	int fine() const{
		if(overtime) return (overtime / 30 + 1) * 30;
		return 0;
	}
	DailyActivity(){}
	friend reporter& operator<<(reporter& o, const DailyActivity& d){
		if(d.driventime){
			o("Date", d.start.datestr());
			if(d.driver.size() >= 2 - (unsigned int)o.verbose){
				if(o.verbose) o.reportray(d.driver,"Daily Activity Driver");
				o("Activities Driver", visualization(d.driver));
			}
			if(d.codriver.size() >= 2 - (unsigned int)o.verbose){
				if(o.verbose) o.reportray(d.codriver,"Daily Activity Codriver");
				o("Activities Codriver",visualization(d.codriver));
			}
			o("Driven time", Activity::formatDurTime(d.driventime));
			if(d.overtime){
				o("Overtime", Activity::formatDurTime(d.overtime));
				o("Fine / EURO", d.fine() );
			}
		}
		return o;
	}
};


#endif
