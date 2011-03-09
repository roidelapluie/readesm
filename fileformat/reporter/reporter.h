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

#ifndef REPORTER_H
#define REPORTER_H REPORTER_H

#include <QtCore/QString>
#include <QTextStream>
#include <QCoreApplication>
#include <QObject>
#include <QSharedPointer>

#include "../picgen.h"



class reporter {
	Q_DECLARE_TR_FUNCTIONS(reporter)

	protected:
	QString collected;
	QTextStream collector;
	public:
	typedef QSharedPointer<picgen> pgptr;
	reporter(const QString& title_ = "ESM Data");
	
	class subblock {
		public:
		subblock(const QString& description, int blockcount, reporter* nparent) :
			empty(blockcount == 0), entriestogo(blockcount), parent(nparent) {
			if(!empty) parent->blockstart(description, blockcount);
		}
		~subblock() {
			if(!empty) parent->blockend();
		}
		subblock& operator++() {
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
	
	subblock newsub(const QString& description, int blockcount);
	
	virtual QString str();
	
	virtual void blockstart(const QString& description, int blockcount) = 0;
	virtual void blockend() = 0;
	virtual void blockbreak() = 0;
	virtual void single(const QString& description, bool ishead = false) = 0;
	virtual void operator()(const QString& description, const QString& value) = 0;
	virtual void operator()(const QString& description, int value) = 0;

	template <typename T>
	void reportray(const T& ray, const QString& description) {
		blockstart(description, ray.size());
		for(typename T::const_iterator i = ray.begin(); i < ray.end(); ++i) {
			(*this) << *i;
			blockbreak();
		}
		blockend();
	}

	template <typename T>
	void reportraynosub(const T& ray) {
		for(typename T::const_iterator i = ray.begin(); i < ray.end(); ++i) {
			(*this) << *i;
			bigblockbreak();
		}
	}
	virtual void bigblockstart(const QString& name) = 0;
	virtual void bigblockbreak();
	virtual void bigblockend() = 0;

	///returns wether embedding images is possible
	virtual bool hasBarGraph() const;
	
	///determines in what way images should be created
	virtual pgptr getBarGraph() const;
	
	virtual bool hasPlotGraph() const;
	virtual pgptr getPlotGraph() const;
	QString title;
	bool verbose;
	QString InfoAppend(bool html = false, bool justname = false);
};

#endif
