// imagemethods.h : methods for reading and writing .PGM files

#ifndef _imagemethods
#define _imagemethods

double* readImage(char* file, int r, int c);
void writeImageToFile(char* file, double* data, int r, int c, int q);

#endif