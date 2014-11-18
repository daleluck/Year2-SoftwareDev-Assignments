#include <iostream>
#include "binaryimage.h"

using namespace std;

//////////////////////////////////
// BinaryImage : IMPLEMENTATION //
//////////////////////////////////

// default constructor
BinaryImage::BinaryImage(int M, int N, double* input_data, double thresh) : Matrix(M, N, input_data)
{
	for ( int i = 0; i < M; i++ )			// traverse rows
	{
		for ( int j = 0; j < N; j++ )		// traverse columns
		{
			// sets to 1 or 0 based on the threshold value
			if ( get(i, j) > thresh ) set(i, j, 1);
			else set(i, j, 0);
		}
	}

	// output message to the console
	cout << "BinaryImage::BinaryImage(int, int, double*, double) is invoked..." << endl;
}

// copy constructor
BinaryImage::BinaryImage(const BinaryImage& biB):Matrix(biB)
{
	// output message to the console
	cout << "BinaryImage::BinaryImage(const BinaryImage&) was invoked..." << endl;
}

// destructor
BinaryImage::~BinaryImage()
{
	// output message to the console
	cout << "BinaryImage::~BinaryImage() is invoked..." << endl;
}

// operator+() / OR function
BinaryImage BinaryImage::operator+(const BinaryImage& biB)
{
	// creates new binary image with same boundaries as this
	BinaryImage C = *this;

	// populate the values held within the image
	for ( int i = 0; i < getM() && i < biB.getM(); i++ )		// traverse rows
	{
		for ( int j = 0; j < getM() && j < biB.getM(); j++ )	// traverse columns
		{
			// if either has the value '1', then assign value '1', else '0'
			C.set(i, j, (get(i,j) == 1 || biB.get(i,j) == 1) ? 1 : 0 );
		}
	}

	// returns new binary image
	return C;
}

// operator-() / XOR function
BinaryImage BinaryImage::operator-(const BinaryImage& biB)
{
	// creates new binary image with same boundaries as this
	BinaryImage C = *this;

	// populate the values held within the image
	for ( int i = 0; i < getM() && i < biB.getM(); i++ )		// traverse rows
	{
		for ( int j = 0; j < getN() && j < biB.getN(); j++ )	// traverse columns
		{
			// if ONLY a single one has the value '1', then assign value '1', else '0'
			C.set(i, j, ((get(i,j) == 1 && biB.get(i,j) == 0) || (get(i,j) == 0 && biB.get(i,j) == 1)) ? 1 : 0 );
		}
	}

	// returns new binary image
	return C;
}

// operator*() / AND function
BinaryImage BinaryImage::operator*(const BinaryImage& biB)
{
	// creates new binary image with same boundaries as this
	BinaryImage C = *this;

	// populate the values held within the image
	for ( int i = 0; i < getM() && i < biB.getM(); i++ )		// traverse rows
	{
		for ( int j = 0; j < getN() && j < biB.getN(); j++ )	// traverse columns
		{
			// if both have the value '1', then assign '1', else '0'
			C.set(i, j, (get(i,j) == 1 && biB.get(i,j) == 1) ? 1 : 0 );
		}
	}

	// returns new binary image
	return C;
}

// operator/() / NOT AND function
BinaryImage BinaryImage::operator/(const BinaryImage& biB)
{
	// creates new binary image with same boundaries as this
	BinaryImage C = *this;

	// populate the values held within the image
	for ( int i = 0; i < getM() && i < biB.getM(); i++ )		// traverse rows
	{
		for ( int j = 0; j < getN() && j < biB.getN(); j++ )	// traverse columns
		{
			// if ONLY a single one has the value '1', then assign value '0', else '1'
			C.set(i, j, ((get(i,j) == 1 && biB.get(i,j) == 0) || (get(i,j) == 0 && biB.get(i,j) == 1)) ? 0 : 1 );
		}
	}

	// returns new binary image
	return C;
}

// operator++() / XOR function
BinaryImage& BinaryImage::operator++()
{
	// populate the values held within the image
	for ( int i = 0; i < getM(); i++ )		// traverse rows
	{
		for ( int j = 0; j < getN(); j++ )	// traverse columns
		{
			// flips every value from 1 to 0, and vice versa
			set(i, j, get(i, j) == 0 ? 1 : 0);
		}
	}

	// returns binary image
	return *this;
}