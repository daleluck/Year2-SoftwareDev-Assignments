#include <sstream>		// stringstream
#include <iostream>		// cout, cerr
#include <fstream>		// ifstream
#include <istream>

using namespace std;

// reads in a .txt file representing an image, name 'fileName', size rows x cols
// converts to a 1D array of doubles
double* readImage(char* fileName, int rows, int cols)
{
	// sets aside the memory for the image in heap
	double* imageArray = new double[rows * cols];

	// opens the file
	ifstream txtFile(fileName);

	// if the file was successfuly opened...
	if ( txtFile.is_open() )
	{
		// counter, used for pointing to correct address in imageArray
		int i = 0;

		// while there's still more to be read from the file...
		while ( !txtFile.eof() )	// (/.good())
		{
			// if there's no more space in the array, stop reading file
			if ( i >= rows * cols ) break;

			// put the next value into the array, and increment 'i'
			txtFile >> *(imageArray + i++);
		}

		// close the file
		txtFile.close();
	}
	// otherwise output an error message to the console
	else cout << "ERROR: unable to open file for reading." << endl;

	// returns pointer to data
	return imageArray;
}

// takes an array of doubles in heap memory, converts it into a .PGM file
void writeImageToFile(char* fileName, double* data, int rows, int cols, int q)
{
	// sets aside the right amount of memory in heap for converting the doubles to chars
	unsigned char* newImage = new unsigned char[rows * cols];

	// actually converts the doubles in 'data' to unsigned chars in 'newImage'
	// note: unsigned chars necessary, since they have value of 0 to 255, not -128 to 127
	for ( int i = 0; i < rows * cols; i++ )
		newImage[i] = (unsigned char)data[i];

	// opens the file location for writing to (consider moving declaration to top)
	ofstream newFile;
	newFile.open(fileName, ios::out|ios::binary|ios::trunc);

	// if the file was unable to be written...
	if ( !newFile )
	{
		// outputs an error message to the console
		cout << "ERROR: unable to open file for writing." << endl;

		// exits out of process and cleans up memory
		exit(1);
	}

	newFile << "P5" << endl;						// specifies file format
	newFile << cols << " " << rows << endl;			// specifies image width and height
	newFile << q << endl;							// specifies maximum pixel value (max = white, 0 = black)

	// writes the rest of the image data into the file
	newFile.write((char*)newImage, (rows * cols) * sizeof(unsigned char));

	// if there's a problem with writing the file...
	if ( newFile.fail() )
	{
		// output a message to the console
		cout << "ERROR: Unable to write image to PGM file." << endl;
		exit(0);
	}

	// close file
	newFile.close();

	// delete image data from heap
	delete[] newImage; newImage = 0;
}