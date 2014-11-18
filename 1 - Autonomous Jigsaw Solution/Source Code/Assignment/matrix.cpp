#include <iostream>
#include "matrix.h"

using namespace std;

/////////////////////////////
// Matrix : IMPLEMENTATION //
/////////////////////////////

// constructor 1 : initializes rows, columns, and sets value across all addresses
Matrix::Matrix(int sizeR, int sizeC, double val)
{
	M = sizeR;							// sets number of rows
	N = sizeC;							// sets number of columns
	data = new double[M	* N];			// sets aside right amount of space in heap for data

	for ( int i = 0; i < M * N; i++ )	// traverses array's memory addresses
	{
		*(data+i) = val;				// initializes each address with the passed value ('val')
	}

	// output message to the console
	cout << "Matrix::Matrix(int, int, double) is invoked..." << endl;
}

// constructor 2 : initializes rows, columns, and copies over data values from a previous Matrix
Matrix::Matrix(int sizeR, int sizeC, double* input_data)
{
	M = sizeR;							// sets number of rows
	N = sizeC;							// sets number of columns
	data = new double[M	* N];			// sets aside right amount of space in heap for data

	for ( int i = 0; i < M * N; i++ )	// traverses array
	{
		*(data + i) = *(input_data + i);	// copies value over to new Matrix
	}

	// output message to the console
	cout << "Matrix::Matrix(int, int, double*) is invoked..." << endl;
}

// copy constructor
Matrix::Matrix(const Matrix& mA)
{
	// sets the boundaries of this matrix to match mA's
	M = mA.getM(); N = mA.getN();

	// set aside the right amount of memory
	data = new double[M * N];

	// populates the data values (deep copy)
	for ( int i = 0; i < M * N; i ++ )
		*(data + i) = mA.get(i);

	// output message to the console
	cout << "Matrix::Matrix(const Matrix&) is invoked..." << endl;
}

// destructor
Matrix::~Matrix()
{
	// clear all the data within the Matrix
	delete[] data; data = 0;

	// output message to the screen
	cout << "Matrix::~Matrix() is invoked..." << endl;
}

// int get_() : returns either M or N, depending on the function called
int Matrix::getM() const
{
	return M;
}
int Matrix::getN() const
{
	return N;
}
double* Matrix::getData() const
{
	return data;
}

// double const get(int, int) : returns specific value from a selected point in the array
double Matrix::get(int i, int j) const
{
	return *(data + (i * N) + j);
}

// double const get(int, int) : returns specific value from a selected point in the array
double Matrix::get(int i) const
{
	return *(data + i);
}

// void set(int, int, double) : sets specific value at a selected point in the array
void Matrix::set(int i, int j, double val)
{
	*(data + (i * N) + j) = val;
}

// void set(int, double) : sets specific value at selected point
void Matrix::set(int i, double val)
{
	data[i]= val;
}

// void show() : outputs the contents of the array to the console
void Matrix::show()
{
	for ( int i = 0; i < M; i++ )		// traverse rows in array
	{
		for ( int j = 0; j < N; j++ )	// traverse columns in array
		{
			//cout << *(data + (i*N) + j) << "\t";
			cout << get(i,j) << "\t";
		}
		cout << endl;
	}
}

// double* getBlock() : returns a pointer to a copied sub-array of the Matrix's array
double* Matrix::getBlock(int start_row, int end_row, int start_col, int end_col) const
{
	// store the number of rows and columns in the subarray
	int newM = (end_row - start_row) + 1;
	int newN = (end_col - start_col) + 1;

	// set aside the right amount of memory
	double* newData = new double[newM * newN];

	// populate the array with the correct values
	for ( int i = 0; i < newM; i++ )		// traverse rows
	{
		for ( int j = 0; j < newN; j++ )	// traverse columns
		{
			*(newData + (i * newN) + j) = get(start_row + i, start_col + j);
		}
	}

	// return pointer to the array
	return newData;
}

// operator functions
Matrix Matrix::operator+(const Matrix& B)
{
	Matrix C(M, N, 0.0);		// create new matrix

	for ( int i = 0; i < M && i < B.getM(); i++ )	// traverse rows
	{
		for ( int j = 0; j < N && j < B.getN(); j++ )	// traverse columns
		{
			// add the two values together and store in C
			C.set(i, j, (*this).get(i, j) + B.get(i, j));
		}
	}

	// output message to the console
	cout << "Matrix::operator+() was invoked...";
	// return new matrix
	return C;
}
Matrix Matrix::operator-(const Matrix& B)
{
	Matrix C(M, N, 0.0);		// create new matrix

	for ( int i = 0; i < M && i < B.getM(); i++ )	// traverse rows
	{
		for ( int j = 0; j < N && j < B.getN(); j++ )	// traverse columns
		{
			// add the two values together and store in C
			C.set(i, j, (*this).get(i, j) - B.get(i, j));
		}
	}
	
	// output message to the console
	cout << "Matrix::operator-() was invoked...";
	// return new matrix
	return C;
}
Matrix Matrix::operator*(const Matrix& B)
{
	Matrix C(M, N, 0.0);		// create new matrix

	for ( int i = 0; i < M && i < B.getM(); i++ )	// traverse rows
	{
		for ( int j = 0; j < N && j < B.getN(); j++ )	// traverse columns
		{
			// add the two values together and store in C
			C.set(i, j, (*this).get(i, j) * B.get(i, j));
		}
	}
	
	// output message to the console
	cout << "Matrix::operator*() was invoked...";
	// return new matrix
	return C;
}
Matrix Matrix::operator/(const Matrix& B)
{
	Matrix C(M, N, 0.0);		// create new matrix

	for ( int i = 0; i < M && i < B.getM(); i++ )	// traverse rows
	{
		for ( int j = 0; j < N && j < B.getN(); j++ )	// traverse columns
		{
			// add the two values together and store in C
			C.set(i, j, (*this).get(i, j) / B.get(i, j));
		}
	}
	
	// output message to the console
	cout << "Matrix::operator/() was invoked...";
	// return new matrix
	return C;
}

Matrix& Matrix::operator=(const Matrix& B)
{
	// output message to the console
	cout << "Matrix::operator=() is invoked...";

	// check to see they're not the same
	if ( this == &B ) return *this;
	// continue on otherwise...
	else
	{
		// delete old data
		delete[] data; data = 0;

		// assign new row and column values
		M = B.getM(); N = B.getN();

		// allocate memory
		data = new double[M * N];

		// populate values of the array
		for ( int i = 0; i < M * N; i++ )
			*(data + i) = B.get(i);

		// return matrix
		return *this;
	}
}
Matrix& Matrix::operator++()
{
	// increment every data value held in the array
	for ( int i = 0; i < M * N; i++ )
		*(data + i) = *(data + i) + 1;

	// output message to the console
	cout << "Matrix::operator++() is invoked...";

	// return matrix
	return *this;
}
double Matrix::operator()(int i, int j)
{
	// output message to the console
	//cout << "Matrix::operator() is invoked...";

	// return value to the caller
	return *(data + (i * N) + j);
}