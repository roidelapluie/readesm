#ifndef SUBBLOCKS_H
#define SUBBLOCKS_H



#include "../constDataPointer.h"
#include "../readTypes.h"
#include "../reporter/reporter.h"

#include <QtCore/QDebug>

#include <vector>

template <typename T>
class Subblocks : public std::vector<T> {
public:
	constDataPointer start_;
	int count_;
	Subblocks(const constDataPointer& start, int count) :start_(start), count_(count)
	{	
		for(int j = 0; j < count_; ++j) push_back(T(start + j*T::staticSize));
	}
	
	Subblocks() : count_(0) {}
	
	int dataSize() const {
		return count_ * T::staticSize;
	}

	void printOn(reporter& o) const{
		o.reportraynosub((*this));
	}
};

#endif
