/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef REPORTER_H
#define REPORTER_H REPORTER_H
#include <sstream>
#include <string>

class reporter : public std::ostringstream{
	public:
	reporter(const std::string& title_ = "ESM Data") : title(title_), verbose(false) {}
	class subblock{
		public:
		subblock(const std::string& description, int blockcount, reporter* nparent) : empty(blockcount == 0), entriestogo(blockcount), parent(nparent){
			if(!empty) parent->blockstart(description, blockcount);
		}
		~subblock(){
			if(!empty) parent->blockend();
		}
		subblock& operator++(){ 
			--entriestogo; 
			if(entriestogo) parent->blockbreak();
			return *this;
		}
		int operator()() { 
			return entriestogo > 0;
		}
		private:
		bool empty;
		int entriestogo;
		reporter* parent;
	};
	subblock newsub(const std::string& description, int blockcount){
		return subblock(description, blockcount, this);
	}
	virtual std::string str(){ return std::ostringstream::str(); }
	virtual void blockstart(const std::string& description, int blockcount) = 0;
	virtual void blockend() = 0;
	virtual void blockbreak() = 0;
	virtual void operator()(const std::string& description, const std::string& value) = 0;
	virtual void operator()(const std::string& description, int value) = 0;
	template <typename T>
	void reportray(const T& ray, const std::string& description){
		blockstart(description, ray.size());
		for(typename T::const_iterator i = ray.begin(); i < ray.end(); ++i){
			(*this) << *i;
			blockbreak();
		}
		blockend();
	}
	template <typename T>
	void reportraynosub(const T& ray){
		for(typename T::const_iterator i = ray.begin(); i < ray.end(); ++i){
			(*this) << *i;
			bigblockbreak();
		}
	}
	virtual void bigblockstart(int index, const std::string& name) = 0;
	virtual void bigblockbreak(){ blockbreak(); };
	virtual void bigblockend() = 0;
	std::string title;
	bool verbose;
};

class txtreporter : public reporter{
	public:
	txtreporter(const std::string& title_ = "ESM Data") : reporter(title_) {}
	virtual std::string str(){ return title + reporter::str(); }
	virtual void blockstart(const std::string& description, int blockcount){
		(*this) << "***************" << description << " (" << blockcount << ") *********\n";
	}
	virtual void blockend(){
		(*this) << "*************************************\n";
	}
	virtual void blockbreak(){
		(*this) << "  ***   \n";
	}
	virtual void operator()(const std::string& description, const std::string& value){
		(*this) << description << ": \t" << value << "\n";
	}
	virtual void operator()(const std::string& description, int value){
		(*this) << description << ": \t" << value << "\n";
	}
	virtual void bigblockstart(int index, const std::string& name){
		(*this) << "\n\n+++++++++++++++++ Block at " << index << " : " << name << " ++++++++++++++++++++++++++\n";
	}
	virtual void bigblockend(){
		(*this) << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	}

};


class htmlreporter : public reporter {
	protected:
	std::ostringstream links;
	public:
	htmlreporter(const std::string& title_ = "ESM Data") : reporter(title_) {}
	virtual std::string str(){
		std::ostringstream o;
		o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n<head><link rel='stylesheet' type='text/css' media='screen' href='style.css'/><title>" << title << "</title><meta http-equiv='Content-Type' content='text/html; charset=latin1'/></head><body><h1>" << title <<"</h1>" << links.str() << "<hr/>" << reporter::str() << "</body></html>\n"; 
		return o.str();
	}
	virtual void bigblockstart(int index, const std::string& name){
		links << "<a href='#" << index <<"'>"<<name<<"</a><br/>";
		(*this) << "<h2><a name='" << index << "'>" << name << "</a></h2><table>";
	}
	virtual void bigblockend(){
		(*this) << "</table>";
	}
	virtual void blockstart(const std::string& description, int blockcount){
		(*this) << "<tr><th>" << description << "</th><td><table>\n";
	}
	virtual void blockbreak(){
		(*this) << "<tr><th></th><td></td></tr>\n";
	}
	virtual void blockend(){
		(*this) << "</table></td></tr>\n";
	}
	virtual void operator()(const std::string& description, const std::string& value){
		(*this) << "<tr><th>" << description << "</th><td>" << value << "</td></tr>\n";
	}
	virtual void operator()(const std::string& description, int value){
		(*this) << "<tr><th>" << description << "</th><td>" << value << "</td></tr>\n";
	}

};

#endif
