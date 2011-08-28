#ifndef SUBBLOCKS_H
#define SUBBLOCKS_H

#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <QtCore/QSharedPointer>

#include "DataType.h"

template <typename T>
class Subblocks : public DataType {
protected:
	QVector<QSharedPointer<T> > array;
	int numberOfBytes;
	void appendAndIncrement(DataPointer& walker, QSharedPointer<T> toAppend){
		if(!toAppend->isDefaultValue()) array.append(toAppend);
		walker += toAppend->size();
	}
	Subblocks(const DataPointer& start)  : DataType(start), numberOfBytes(0) {}
public:
	QSharedPointer<T> operator[](int j) const{
		return array[j];
	}
	int numberOfBlocks() const{
		return array.size();
	}
	void printOn(Reporter& o) const{
		for(int j = 0; j < numberOfBlocks(); ++j) o.namedSubBlock("subblock", *array[j]);
	}
	int size() const {
		return numberOfBytes;
	}
	///Create a fixed number of subblocks
	static Subblocks fromTypeAndCount(const DataPointer& start, int count){
		qDebug() << "starting subblocks";
		DataPointer walker(start);
		Subblocks rv(start);
		for(int j = 0; j < count; ++j) rv.appendAndIncrement(walker, QSharedPointer<T>(new T(walker)));
		rv.numberOfBytes = walker - start;
		return rv;
	}
	///Create Subblocks if a fixed number of bytes is to be read
	static Subblocks fromTypeAndLength(const DataPointer& start, int length){
		DataPointer walker(start);
		Subblocks rv(start);
		while(walker - start < length){
			//qDebug() << "pos " << (walker - start) << " of " << length;
			rv.appendAndIncrement(walker, QSharedPointer<T>(new T(walker)));
		}
		rv.numberOfBytes = walker - start;
		return rv;
	}
};

#endif
