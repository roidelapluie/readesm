#ifndef SUBBLOCKS_H
#define SUBBLOCKS_H

#include <vector>

#include "DataType.h"

template <typename T>
class Subblocks : public DataType {
protected:
	std::vector<T> array;
	int numberOfBytes;
	void appendAndIncrement(DataPointer& walker, T toAppend){
		if(!toAppend.isDefaultValue()) array.push_back(toAppend);
		walker += toAppend.size();
	}
	Subblocks(const DataPointer& start)  : DataType(start), numberOfBytes(0) {}
public:
	const T& operator[](int j) const{
		return array[j];
	}
	T& operator[](int j){
		return array[j];
	}
	int numberOfBlocks() const{
		return array.size();
	}
	void printOn(Reporter& o) const{
		o.writeArray(*this, title());
	}
	int size() const {
		return numberOfBytes;
	}
	///Create a fixed number of subblocks
	static Subblocks fromTypeAndCount(const DataPointer& start, int count){
		DataPointer walker(start);
		Subblocks rv(start);
		for(int j = 0; j < count; ++j) rv.appendAndIncrement(walker, T(walker));
		rv.numberOfBytes = walker - start;
		return rv;
	}
	///Create Subblocks if a fixed number of bytes is to be read
	static Subblocks fromTypeAndLength(const DataPointer& start, int length){
		DataPointer walker(start);
		Subblocks rv(start);
		while(walker - start < length){
			//qDebug() << "pos " << (walker - start) << " of " << length;
			rv.appendAndIncrement(walker, T(walker));
		}
		rv.numberOfBytes = walker - start;
		return rv;
	}
};

#endif
