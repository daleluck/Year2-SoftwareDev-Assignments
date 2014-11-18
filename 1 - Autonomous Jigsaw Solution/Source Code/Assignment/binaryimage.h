// binaryimage.h : derived class of matrix, allows only binary values (0/1)
#include "matrix.h"

#ifndef _binaryimage
#define binaryimage

class BinaryImage : public Matrix
{
public:
	BinaryImage(int, int, double*, double);		// constructor
	BinaryImage(const BinaryImage&);			// copy constructor
	~BinaryImage();								// destructor

	BinaryImage operator+(const BinaryImage&);	// OR function
	BinaryImage operator-(const BinaryImage&);	// XOR function
	BinaryImage operator*(const BinaryImage&);	// AND function
	BinaryImage operator/(const BinaryImage&);	// NAND function

	BinaryImage& operator++();					// NOT function
};

#endif