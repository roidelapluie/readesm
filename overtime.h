/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef OVERTIME_H
#define OVERTIME_H OVERTIME_H
#include "Activity.h"
#include "typedefs.h"
int fine(int overtime){
	if(overtime) return (overtime / 30 + 1) * 30;
	return 0;
}

void checkDayDrivingTime(DailyActivity& a){
	typedef std::vector<Activity> subray;
	typedef subray::const_iterator subiter;
	int sincelastbreak = 0;
	int overtime = 0;
	bool had15minbreak = false;
	for(subiter i = a.driver.begin(); i < a.driver.end(); ++i){
		const int& d = i->duration;
		if(i->activity == Activity::Driving){
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
	a.overtime = std::max(overtime, a.driventime - 600);
	a.fine = fine(a.overtime);
	a.overtimeReason = "Daily driving time";
}


#endif