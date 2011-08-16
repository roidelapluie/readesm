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

#ifndef ESMFILE_H
#define ESMFILE_H


#include "Block.h"

#include <QtCore/QString>
#include <QVector>
#include <QSharedPointer>
#include <QByteArray>



class ESMFile {
	Q_DECLARE_TR_FUNCTIONS(ESMFile)

public:
	QByteArray content;
	QVector< QSharedPointer<Block> > blocks;

	ESMFile(const QString& filename);

	QString suggestTitle() const;
	QString suggestFileName() const;
	
	friend reporter& operator<<(reporter& report, const ESMFile& e);
	
protected:
	void printOn(reporter& o) const;

};

#endif
