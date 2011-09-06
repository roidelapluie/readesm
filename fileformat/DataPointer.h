#ifndef DATAPOINTER_H
#define DATAPOINTER_H

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QString>
class DataPointer {
QString hexByte(unsigned char start) const{
	return QString("%1").arg(start, 2, 16, QChar('0'));
}
	public:
	QByteArray data;
	int offset;

	explicit DataPointer(const QByteArray& data_, int offset_ = 0) :
		data(data_),
		offset(offset_)
	{
	}

	inline DataPointer operator+(int j) const
	{
		return DataPointer(data, offset + j);
	}

	DataPointer operator-(int j) const
	{
		return DataPointer(data, offset - j);
	}

	int operator-(const DataPointer& other) const
	{
		return offset - other.offset;
	}

	DataPointer& operator+=(int j)
	{
		offset += j;
		return *this;
	}

	inline unsigned char operator[](int j) const
	{
		return data[offset + j];
	}

	const unsigned char* toUnsignedPointer(int j = 0) const
	{
		return reinterpret_cast<const unsigned char*>(toPointer(j));
	}

	const char* toPointer(int j = 0) const
	{
		return data.data() + offset + j;
	}

	int bytesLeft() const
	{
		return data.size() - offset;
	}

	bool operator<(const DataPointer& other) {
		return offset < other.offset;
	}
	
	QByteArray toQByteArray() const;
};
#endif
