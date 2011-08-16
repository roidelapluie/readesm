#ifndef SUBBLOCKS_H
#define SUBBLOCKS_H


#include <vector>
#include "../constDataPointer.h"
#include "../readTypes.h"
#include "../reporter/reporter.h"
#include <QtCore/QDebug>

template <typename T>
class Subblocks : public std::vector<T> {
public:
	constDataPointer start_;
	int count_;
	Subblocks(const constDataPointer& start, int count) :start_(start), count_(count)
	{	
		qDebug() << count << "subblocks have each" << T::staticSize << "size " << count_ * T::staticSize;
		for(int j = 0; j < count_; ++j) push_back(T(start + j*T::staticSize));
	}
	
	int dataSize() const {
		return count_ * T::staticSize;
	}

	void printOn(reporter& o) const{
		for(unsigned int j = 0; j < this->size(); ++j) (*this)[j].printOn(o);
	}
};

#endif