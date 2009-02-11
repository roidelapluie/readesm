/*
 *	sha1.h
 *
 *	Copyright (C) 1998
 *	Paul E. Jones <paulej@arid.us>
 *	All Rights Reserved.
 *
 *****************************************************************************
 *	$Id: sha1.h,v 1.6 2004/03/27 18:02:26 paulej Exp $
 *****************************************************************************
 *
 *	Description:
 * 		This class implements the Secure Hashing Standard as defined
 * 		in FIPS PUB 180-1 published April 17, 1995.
 *
 *		Many of the variable names in this class, especially the single
 *		character names, were used because those were the names used
 *		in the publication.
 *
 * 		Please read the file sha1.cpp for more information.
 *
 */
//This file was cruelly changed by Andreas Goelzer.
//Get The original version at http://www.packetizer.com/security/sha1/
//TODO: clean this up again
//see license-sha1.txt foor license information
#ifndef _SHA1_H_
#define _SHA1_H_

#include <iostream>
#include <iomanip>

class SHA1
{

	public:

		SHA1();
		virtual ~SHA1();

		/*
		 *	Re-initialize the class
		 */
		void Reset();

		/*
		 *	Returns the message digest
		 */
		bool Result(unsigned *message_digest_array);

		/*
		 *	Provide input to SHA1
		 */
		SHA1(const unsigned char *message_array, unsigned length){
			Reset();
			Input(message_array, length);
		}
		void Input(	const unsigned char	*message_array,
					unsigned			length);
		void Input(	const char	*message_array,
					unsigned	length);
		void Input(unsigned char message_element);
		void Input(char message_element);
		SHA1& operator<<(const char *message_array);
		SHA1& operator<<(const unsigned char *message_array);
		SHA1& operator<<(const char message_element);
		SHA1& operator<<(const unsigned char message_element);
		bool checkmatch(const unsigned char *charray){
			if (Corrupted) return false;
			if (!Computed){
				PadMessage();
				Computed = true;
			}
			for(int j = 0; j < 5; ++j){
				if((H[j] & 0xFF) != charray[4*j + 3]) return false;
				if(((H[j] >> 8) & 0xFF) != charray[4*j + 2]) return false;
				if(((H[j] >> 16) & 0xFF) != charray[4*j + 1]) return false;
				if(((H[j] >> 24) & 0xFF) != charray[4*j + 0]) return false;
			}
			return true;
		}
		void Result(unsigned char *charray){
			if (!Computed){
				PadMessage();
				Computed = true;
			}
			for(int j = 0; j < 5; ++j){
				charray[4*j + 3] = H[j] & 0xFF;
				charray[4*j + 2] = (H[j] >> 8) & 0xFF;
				charray[4*j + 1] = (H[j] >> 16) & 0xFF;
				charray[4*j + 0] = (H[j] >> 24) & 0xFF;
			}
		}
		friend std::ostream& operator<<(std::ostream& o, SHA1 s){
			if(!s.Computed){
				s.PadMessage();
				s.Computed = true;
			}
			o << std::hex << std::setw(2) << std::setfill('0');
			for(int j = 0; j < 5; ++j){
				o << (s.H[j] & 0xFF);
				o << ((s.H[j] >> 8) & 0xFF);
				o << ((s.H[j] >> 16) & 0xFF);
				o << ((s.H[j] >> 24) & 0xFF);
			}
			return o;
		}

	private:

		/*
		 *	Process the next 512 bits of the message
		 */
		void ProcessMessageBlock();

		/*
		 *	Pads the current message block to 512 bits
		 */
		void PadMessage();

		/*
		 *	Performs a circular left shift operation
		 */
		inline unsigned CircularShift(int bits, unsigned word);

		unsigned H[5];						// Message digest buffers

		unsigned Length_Low;				// Message length in bits
		unsigned Length_High;				// Message length in bits

		unsigned char Message_Block[64];	// 512-bit message blocks
		int Message_Block_Index;			// Index into message block array

		bool Computed;						// Is the digest computed?
		bool Corrupted;						// Is the message digest corruped?
	
};

#endif
