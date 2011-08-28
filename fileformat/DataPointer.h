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

	DataPointer(const DataPointer& other) :
		data(other.data),
		offset(other.offset)
	{
	}

	DataPointer& operator=(const DataPointer& other)
	{
		data = other.data;
		offset = other.offset;
		return *this;
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
		if(j > bytesLeft()) qDebug() << "error, reading past end of file" << offset  << j << bytesLeft() << data.size() << hexByte(data[1]) << hexByte(data[2]);
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
