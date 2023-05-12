#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Dataset.h"


/* Configure size of array and, as such, amount of operations */
#ifndef SIZE
#define SIZE 64
#endif

/* Becase we might wanna test different properties, we can define here a diferent build type */
// Using doubles
#ifndef TYPE
#define TYPE 1
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

// Using doubles
#elif TYPE == 4
#define DataType double
#define DataSet fDataset
#define DataFormat(start, end) start "%3.4lf" end

#else
#error *Did not pass a type of value 1, 2, 3 or 4*
#endif // TYPE

void initArray(DataType array[SIZE]);
void initArray2D(DataType array[SIZE][SIZE]);

void initZero(DataType array[SIZE]);


#endif // FUNCTIONS_H