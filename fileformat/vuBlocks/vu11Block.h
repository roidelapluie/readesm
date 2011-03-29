#ifndef VU11BLOCK_H
#define VU11BLOCK_H

#include "config.h"
#include "vuBlock.h"
#include "../helper.h"

#ifndef HAVE_NO_CRYPTO
#include "../crypto.h"
#endif

#include <QtCore/QString>
#include <QtCore/QTextStream>




class block11Record {
	public:
	QString cardNumber;
	Time timeOne, timeTwo;
	QString payload;
	block11Record(constDataPointer start) : 
		cardNumber(fixedString(start,17)),
		timeOne(BEInt32(start + 18)),
		timeTwo(BEInt32(start + 22)),
		payload(hexchunk(start + 26, 32))
	{
		qDebug() << start[0] << start[1];
	}
	friend reporter& operator<<(reporter& o, const block11Record& data){
		o("cardNumber", data.cardNumber);
		o("1st Time", data.timeOne.str());
		o("2nd Time", data.timeTwo.str());
		o("data: ", "<pre>" + data.payload + "</pre>");
		return o;
	}
		
};

/** This block is for debugging, in case the program encounters
 * some data it so far cannot read, it reports hex data and as
 * many useful information as possible **/
class vu11Block : public vuBlock {
	public:
	static const int TREP = 0x11;
	
	int numberOfRecords;

	vu11Block(constDataPointer start) :
		vuBlock(start),
		numberOfRecords(BEInt16(start + 17))
	{
		hassignature = false;
		qDebug() << numberOfRecords;
	}
	
	virtual int size() const {
		int temp = 19 + numberOfRecords * 58;
		return temp;
		//return start.bytesLeft();
	}
	
	virtual QString name() const{
		return QString("Unknown block in ADDs, TREP %1").arg(TREP);
	}
	virtual void CompleteReport(reporter& o) const {
		qDebug() << "reporting" << numberOfRecords;
		o.single("This block is not covered in the european law. My(Andreas) guess is that this is something manufacturer-specific, in the files from siemens vdos this block (code 0x7611) contains, well, card numbers and timestamps. It might be something else from other manufacturers. If you do know what it does, please tell me.");
		o("Block offset", QString("%1 (0x%2)")
					.arg(start.offset)
					.arg(start.offset, 8, 16, QChar('0')));
		o("Block size", QString("%1").arg(size()));
		o.single("Block start data: " + hexchunk(start, 20));
		o("NumberOfRecords", numberOfRecords);
		for(int j = 0; j < numberOfRecords; ++j){
			block11Record temp(start + 19 + j * 58);
			o << temp;
		}
	}
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa&){
		return false;
	}
#endif
};

/** This block is for debugging, in case the program encounters
 * some data it so far cannot read, it reports hex data and as
 * many useful information as possible **/
class vu13Block : public vuBlock {
	public:
	static const int TREP = 0x13;
	
	int numberOfRecords;

	vu13Block(constDataPointer start) :
		vuBlock(start),
		numberOfRecords(BEInt16(start + 32))
	{
		hassignature = false;
		qDebug() << numberOfRecords;
	}
	
	virtual int size() const {
		int temp = 34 + numberOfRecords * 58;
		return temp;
		//return start.bytesLeft();
	}
	
	virtual QString name() const{
		return QString("Unknown block in ADDs, TREP %1").arg(TREP);
	}
	virtual void CompleteReport(reporter& o) const {
		qDebug() << "reporting" << numberOfRecords;
		o.single("This block is not covered in the european law. My(Andreas) guess is that this is something manufacturer-specific, in the files from siemens vdos this block (code 0x7611) contains, well, card numbers and timestamps. It might be something else from other manufacturers. If you do know what it does, please tell me.");
		o("Block offset", QString("%1 (0x%2)")
					.arg(start.offset)
					.arg(start.offset, 8, 16, QChar('0')));
		o("Block size", QString("%1").arg(size()));
		o.single("Block start data: " + hexchunk(start, 35));
		o("NumberOfRecords", numberOfRecords);
		for(int j = 0; j < numberOfRecords; ++j){
			if(start.bytesLeft() < 34 + (j + 1)* 58) break;
			block11Record temp(start + 34 + j * 58);
			o << temp;
		}
	}
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa&){
		return false;
	}
#endif
};

#endif
