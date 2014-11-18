#include <iostream>
#include <math.h>
#include "matrix.h"
#include "binaryimage.h"
#include "imagemethods.h"

using namespace std;

double* unshuffleImage(BinaryImage&, BinaryImage&, int, int);
double* findTemplate_SAD(Matrix&, Matrix&);
double* findTemplate_SSD(Matrix&, Matrix&);

int main()
{
	////////////////////////////////
	// TASK 1 - Unshuffled Logo   //
	////////////////////////////////

	// setup for task 1
	cout << "Loading images for task 1..." << endl;
	double* noisyImage = readImage("Task 1/logo_with_noise.txt", 512, 512);
	double* shuffledImage = readImage("Task 1/logo_shuffled.txt", 512, 512);

	// convert images to binary (locality sensitive hashing)
	cout << "Converting images to binary format..." << endl;
	BinaryImage noisyImageBin(512, 512, noisyImage, 150);
	BinaryImage shuffledImageBin(512, 512, shuffledImage, 200);

	// unshuffle image and write to file
	cout << "Unshuffling logo..." << endl;
	double* result_task1 = unshuffleImage(noisyImageBin, shuffledImageBin, 512, 512);
	writeImageToFile("Task 1/unshuffled.PGM", result_task1, 512, 512, 1);
	cout << "Logo saved to 'Task 1/unshuffled.PGM'" << endl;

	// cleanup for task 1
	cout << "Cleaning up task 1 memory..." << endl;
	delete[] noisyImage; noisyImage = 0;
	delete[] shuffledImage; shuffledImage = 0;
	delete[] result_task1; result_task1 = 0;

	//////////////////////////////////
	// TASK 2 - Template Matching   //
	//////////////////////////////////

	// setup for task 2
	cout << "Loading images for task 2..." << endl;
	double* imageHolder = readImage("Task 2/Cluttered_scene.txt", 768, 1024);
	double* templateHolder = readImage("Task 2/Wally_grey.txt", 49, 36);
	
	// convert data to Matrix format
	Matrix image(768, 1024, imageHolder);
	Matrix templateImage(49, 36, templateHolder);
	
	// task 2.1 - sum of absolute differences
	cout << "Running SAD implementation of task 2 (this can take a while)..." << endl;
	double* cropped = findTemplate_SAD(image, templateImage);
	writeImageToFile("Task 2/croppedImage_SAD.pgm", cropped, 49, 36, 255);
	cout << "Result saved to 'Task 2/croppedImage_SAD.PGM'" << endl;

	// task 2.2 - sum of squared differences
	cout << "Running SSD implementation of task 2 (this can take a while)..." << endl;
	double* cropped2 = findTemplate_SSD(image, templateImage);
	writeImageToFile("Task 2/croppedImage_SSD.pgm", cropped2, 49, 36, 255);
	cout << "Result saved to 'Task 2/croppedImage_SSD.PGM'" << endl;
	
	// clean up memory
	cout << "Cleaning up task 2 memory..." << endl;
	delete[] imageHolder; imageHolder = 0;
	delete[] templateHolder; templateHolder = 0;
	delete[] cropped; cropped = 0;
	delete[] cropped2; cropped2 = 0;

	// finish program, return standard exit integer (0)
	system("pause");
	return 0;
}

double* unshuffleImage(BinaryImage& image, BinaryImage& shuffled, int width, int height)
{
	// creater new array of doubles to hold the output
	double* unshuffled = new double[width * height];

	int bestValue = 0, holderValue = 0;
	int row = 0, col = 0;

	// look through each block of the original image
	for ( int i = 0; i < height; i+=32 )
	{
		for ( int j = 0; j < width; j+=32 )
		{
			// and compare against each block in the shuffled image
			for ( int i2 = 0; i2 < height; i2+=32 )
			{
				for ( int j2 = 0; j2 < width; j2+=32 )
				{
					// and within that, calculate how many of the pixels match...
					for ( int a = 0; a < 32; a++ )
					{
						for ( int b = 0; b < 32; b++ )
						{
							if ( a > 5 && a < 27 && b > 5 && b < 27 )
								continue;

							if ( image(i+a, j+b) == shuffled(i2+a, j2+b) )
								holderValue += 1;
						}
					}

					// if it's better than the previous best...
					if ( holderValue > bestValue )
					{
						// store the appropriate values
						bestValue = holderValue;
						row = i2; col = j2;
					}
					
					holderValue = 0;
				}
			}

			// store the correct block details in 'unshuffled'
			for ( int a = 0; a < 32; a++ )
			{
				for ( int b = 0; b < 32; b++ )
				{
					unshuffled[((i+a)*width) + (j + b)] = shuffled(row+a, col+b);
				}
			}

			// reset values for later
			bestValue = 0; holderValue = 0;
			row = 0; col = 0;
		}
	}

	// return pointer to caller
	return unshuffled;
}

double* findTemplate_SAD(Matrix& image, Matrix& tempImage)
{
	double* finalImage = new double[tempImage.getM() * tempImage.getN()];

	double bestSAD = 0, SAD = 0;
	int x = 0, y = 0;

	// for each part of the original image...
	for ( int i = 0; i < image.getM() - tempImage.getM(); i ++ )
	{
		for ( int j = 0; j < image.getN() - tempImage.getN(); j ++ )
		{
			// calculates the SAD value between each of the pixels
			for ( int i2 = 0; i2 < tempImage.getM(); i2++ )
			{
				for ( int j2 = 0; j2 < tempImage.getN(); j2++ )
				{
					SAD += abs(image(i+i2,j+j2) - tempImage(i2,j2));
				}
			}

			// if this value is better than the previous best, store values
			if ( SAD < bestSAD || (i == 0 && j == 0))
			{
				bestSAD = SAD;
				x = j; y = i;
			}

			// resets the SAD value holder for the next attempt
			SAD = 0;
		}
	}

	// draw the correct part of the original image to finalImage
	for ( int i = y; i < y + tempImage.getM(); i++ )
	{
		if ( i >= image.getM() ) break;
		for ( int j = x; j < x + tempImage.getN(); j++ )
		{
			if ( j >= image.getN() ) break;
			finalImage[((i-y)*tempImage.getN()) + (j-x)] = image(i,j);
		}
	}

	// return the result to the caller as a pointer
	return finalImage;
}

double* findTemplate_SSD(Matrix& image, Matrix& tempImage)
{
	double* finalImage = new double[tempImage.getM() * tempImage.getN()];

	double bestSSD = 0, SSD = 0;
	int x = 0, y = 0;

	// for each part of the original image...
	for ( int i = 0; i < image.getM() - tempImage.getM(); i ++ )
	{
		for ( int j = 0; j < image.getN() - tempImage.getN(); j ++ )
		{
			// calculates the SSD value between each of the pixels
			for ( int i2 = 0; i2 < tempImage.getM(); i2++ )
			{
				for ( int j2 = 0; j2 < tempImage.getN(); j2++ )
				{
					SSD += (image(i+i2,j+j2) - tempImage(i2,j2)) * (image(i+i2,j+j2) - tempImage(i2,j2));
				}
			}

			// if this value is better than the previous best, store values
			if ( SSD < bestSSD || (i == 0 && j == 0))
			{
				bestSSD = SSD;
				x = j; y = i;
			}

			// resets the SSD value holder for the next attempt
			SSD = 0;
		}
	}

	// draw the correct part of the original image to finalImage
	for ( int i = y; i < y + tempImage.getM(); i++ )
	{
		if ( i >= image.getM() ) break;
		for ( int j = x; j < x + tempImage.getN(); j++ )
		{
			if ( j >= image.getN() ) break;
			finalImage[((i-y)*tempImage.getN()) + (j-x)] = image(i,j);
		}
	}

	// return the result to the caller as a pointer
	return finalImage;
}