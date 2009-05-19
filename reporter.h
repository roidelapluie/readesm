/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef REPORTER_H
#define REPORTER_H REPORTER_H
#include "helper.h"
#include "picgen.h"
#include "typedefs.h"
#include "i18n.h"

string InfoAppend(bool html = false){
	string rv = tr("Report generated with") + " ";
	if(html) rv += "<a href='http://readesm.sourceforge.net/'>readESM</a>";
	else rv += "readESM (http://readesm.sourceforge.net/)";
#ifdef VERSION
	rv += " " + tr("version") + " " + VERSION;
#endif
	return rv + "\n";
}

class reporter : public ostringstream{
	public:
	typedef shared_ptr<picgen> pgptr;
	reporter(const string& title_ = "ESM Data") : title(title_), verbose(false) {}
	class subblock{
		public:
		subblock(const string& description, int blockcount, reporter* nparent) : empty(blockcount == 0), entriestogo(blockcount), parent(nparent){
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
	subblock newsub(const string& description, int blockcount){
		return subblock(description, blockcount, this);
	}
	virtual string str(){ return ostringstream::str(); }
	virtual void blockstart(const string& description, int blockcount) = 0;
	virtual void blockend() = 0;
	virtual void blockbreak() = 0;
	virtual void single(const string& description, bool ishead = false) = 0;
	virtual void operator()(const string& description, const std::string& value) = 0;
	virtual void operator()(const string& description, int value) = 0;
	template <typename T>
	void reportray(const T& ray, const string& description){
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
	virtual void bigblockstart(const string& name) = 0;
	virtual void bigblockbreak(){ blockbreak(); };
	virtual void bigblockend() = 0;
	///returns wether embedding images is possible
	virtual bool hasBarGraph() const { return false; }
	///determines in what way images should be created
	virtual pgptr getBarGraph() const { return pgptr(new picgen); }

	virtual bool hasPlotGraph() const { return false; }
	virtual pgptr getPlotGraph() const { return pgptr(new picgen); }
	string title;
	bool verbose;
};

class txtreporter : public reporter{
	public:
	txtreporter(const string& title_ = "ESM Data") : reporter(title_) {}
	virtual string str(){ return title + reporter::str() + InfoAppend(); }
	virtual void blockstart(const string& description, int blockcount){
		(*this) << "***************" << tr(description) << " (" << blockcount << ") *********\n";
	}
	virtual void blockend(){
		(*this) << "*************************************\n";
	}
	virtual void blockbreak(){
		(*this) << "  ***   \n";
	}
	virtual void single(const string& description, bool ishead = false){
		string mark = "";
		if(ishead) mark = "**";
		(*this) << mark << " " << tr(description) << " " << mark << std::endl;
	}
	virtual void operator()(const string& description, const std::string& value){
		(*this) << tr(description) << ": \t" << value << std::endl;
	}
	virtual void operator()(const string& description, int value){
		(*this) << tr(description) << ": \t" << value << std::endl;
	}
	virtual void bigblockstart(const string& name){
		(*this) << "\n\n+++++++++++++++++ Block: " << name << " ++++++++++++++++++++++++++\n";
	}
	virtual void bigblockend(){
		(*this) << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	}

};

///HTML reporter
/** This reporter aims to format the data in a nicer fashion, and creates graphic visualizations
by punching together various stretched single-pixel images */
class htmlreporter : public reporter {
	protected:
	ostringstream links;
	int targetcount;
	public:
	htmlreporter(const string& title_ = "ESM Data") : reporter(title_), targetcount(0) {}
	virtual string str(){
		ostringstream o;
		o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n<head><link rel='stylesheet' type='text/css' media='screen' href='style.css'/><title>" << title << "</title><meta http-equiv='Content-Type' content='text/html; charset=utf8'/></head><body><h1>" << title <<"</h1>" << links.str() << "<hr/>" << reporter::str() << "<hr/>" << InfoAppend(true) << "</body></html>\n";
		return o.str();
	}
	virtual void bigblockstart(const string& name){
		links << "<a href='#" << stringify(++targetcount) <<"'>" << tr(name) << "</a><br/>";
		(*this) << "<h2><a name='" << stringify(targetcount) << "'>" << tr(name) << "</a></h2><table>";
	}
	virtual void bigblockend(){
		(*this) << "</table>";
	}
	virtual void blockstart(const string& description, int blockcount){
		(*this) << "<tr><th>" << tr(description) << "</th><td><table>\n";
	}
	virtual void blockbreak(){
		(*this) << "<tr><th></th><td></td></tr>\n";
	}
	virtual void blockend(){
		(*this) << "</table></td></tr>\n";
	}
	virtual void single(const string& description, bool ishead = false){
		string mark = "td";
		if(ishead) mark = "th";
		(*this) << "<tr><" << mark << " colspan='2' class='center'>" << tr(description) << "</" << mark << "></tr>" << std::endl;
	}
	virtual void operator()(const string& description, const std::string& value){
		(*this) << "<tr><th>" << tr(description) << "</th><td>" << value << "</td></tr>\n";
	}
	virtual void operator()(const string& description, int value){
		(*this) << "<tr><th>" << tr(description) << "</th><td>" << value << "</td></tr>\n";
	}
	virtual bool hasBarGraph() const { return true; }
	virtual pgptr getBarGraph() const { return pgptr(new htmlBarGraph); }
};

///The XHTML(inline svg graphics) reporter class
/** This reporter aims to produce one single file of output with everything embedded within it.
To do that it embeds svg into the html, it works in firefox, not in konqueror :( */
class xmlreporter : public htmlreporter {
	public:
	virtual string str(){
		ostringstream o;
		o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n" \
		"<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n" \
		"<head><title>" << title << "</title>" \
		"<meta http-equiv='Content-Type' content='text/html; charset=utf8'/>" \
		"<style type=\"text/css\">" \
		"table		{ border-collapse:collapse;  border-width:2px; border-style:solid; }" \
		"tr 		{ border-top-width:1px; border-top-style:dashed; }" \
		"tr:hover 	{border-style:outset; background-color:#eeeeee; }" \
		".center	{text-align:center; }" \
		"th		{text-align:left;vertical-align:top;}" \
		"td,th.max,th.avg, th.min{text-align:left;border-left: 1px dotted;}" \
		"caption		{font-size:0.5em;}" \
		"small		{font-size:0.8em;}</style>" \
		"</head><body>" \
		"<h1>" << title <<"</h1>" << links.str() << "<hr/>" \
		<< reporter::str() << "<hr/>" << InfoAppend(true) \
		<< "</body></html>\n";
		return o.str();
	}
	xmlreporter(const string& title_ = "ESM Data") : htmlreporter(title_) {}
	virtual pgptr getBarGraph() const { return pgptr(new svgBarGraph); }
	virtual bool hasPlotGraph() const { return true; }
	virtual pgptr getPlotGraph() const { return pgptr(new svgPlotGraph); }
};

#endif
