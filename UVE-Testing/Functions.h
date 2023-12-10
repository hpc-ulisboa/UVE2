#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include "Dataset.h"


/* Configure size of array and, as such, amount of operations */
#ifndef SIZE
#define SIZE 64
#endif

/* Becase we might wanna test different properties, we can define here a diferent build type */
// Using doubles
#ifndef TYPE
#define TYPE 5
#endif // TYPE

// Using bytes
#if TYPE == 1
#define DataType char
#define DataSet iDataset
#define DataFormat(start, end) start "%x" end

// Using shorts (half-words)
#elif TYPE == 2
#define DataType short
#define DataSet iDataset
#define DataFormat(start, end) start "%hd" end

// Using integers (words)
#elif TYPE == 3
#define DataType int
#define DataSet iDataset
#define DataFormat(start, end) start "%d" end

// Using floats (words)
#elif TYPE == 4
#define DataType float
#define DataSet fDataset
#define sgdX sgdXDataset
#define sgdY sgdYDataset
#define DataFormat(start, end) start "%3.6f" end

// Using doubles
#elif TYPE == 5
#define DataType double
#define DataSet fDataset
#define sgdX sgdXDataset
#define sgdY sgdYDataset
#define DataFormat(start, end) start "%3.6lf" end

#else
#error *Did not pass a type of value 1, 2, 3, 4 or 5*
#endif // TYPE

void initArray(DataType *array, int N);
void initArray2D(DataType *array, int N, int M);

void initZero(DataType *array, int N);
void initConstant(DataType *array, int N, DataType m);


#endif // FUNCTIONS_H