// matrix.h : data member and function definitions for the Matrix class

#ifndef _matrixclass		// if the Matrix class hasn't already been defined...
#define _matrixclass		// ...flag that it has been

class Matrix
{
private:
	int M, N;						// rows, columns
	double* data;					// pointer to actual array contents
public:
	Matrix(int, int, double);		// constructor 1
	Matrix(int, int, double*);		// constructor 2
	Matrix(const Matrix&);			// copy constructor
	~Matrix();						// destructor
	
	int getM() const;				// returns M
	int getN() const;				// returns N
	double* getData() const;		// returns pointer to data

	double get(int, int) const;		// get function 1
	double get(int) const;			// get function 2
	void set(int, int, double);		// set function 1
	void set(int, double);			// set function 2
	void show();					// outputs the contents to console

	double* getBlock(int, int, int, int) const;

	Matrix operator+(const Matrix&);	// addition operator function
	Matrix operator-(const Matrix&);	// minus operator function
	Matrix operator*(const Matrix&);	// multiplication operator function
	Matrix operator/(const Matrix&);	// division operator function

	Matrix& operator=(const Matrix&);	// equals operator function
	Matrix& operator++();				// increment operator function
	double operator()(int, int);		// "subscript" operator function

};

#endif