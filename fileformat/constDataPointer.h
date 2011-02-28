#ifndef CONSTDATAPOINTER_H
#define CONSTDATAPOINTER_H

#include <QByteArray>

class constDataPointer {
	QByteArray data;
	int offset;
	public:
	constDataPointer(){}
	
	constDataPointer(const QByteArray& data_, int offset_ = 0) :
		data(data_),
		offset(offset_)
	{
	}

	constDataPointer operator+(int j) const
	{
		return constDataPointer(data, offset + j);
	}

	constDataPointer operator-(int j) const
	{
		return constDataPointer(data, offset - j);
	}
	
	constDataPointer& operator+=(int j)
	{
		offset += j;
		return *this;
	}

	char operator[](int j) const
	{
		return data[offset + j];
	}

// 	operator const char * () const{
// 		return data + offset;
// 	}

	const unsigned char* toUnsignedPointer(int j = 0) const
	{
		return reinterpret_cast<const unsigned char*>(data.data() + offset + j);
	}

	const char* toPointer(int j = 0) const
	{
		return data.data() + offset + j;
	}
	
	int bytesLeft() const
	{
		return data.size() - offset;
	}
	
	bool operator<(const constDataPointer& other){
		return offset < other.offset;
	}
};
#endif
