#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include <stdio.h>
#include "Dataset.h"


/* Configure size of array and, as such, amount of operations */
#ifndef SIZE
#define SIZE 64
#endif

/* Becase we might wanna test different properties, we can define here a diferent build type */
// Using bytes
#ifdef B_TYPE
#define DataType signed char
#define DataSet iDataset
#define DataFormat(start, end) start "%hhd" end
#endif // BYTE_TYPE

// Using shorts (half-words)
#ifdef H_TYPE
#define DataType short
#define DataSet iDataset
#define DataFormat(start, end) start "%hd" end
#endif // HALF_WORD_TYPE

// Using integers (words)
#ifdef I_TYPE
#define DataType int
#define DataSet iDataset
#define DataFormat(start, end) start "%d" end
#endif // INT_TYPE

// Using floats (words)
#ifdef F_TYPE
#define DataType float
#define DataSet fDataset
#define sgdX sgdXDataset
#define sgdY sgdYDataset
#define DataFormat(start, end) start "%3.6f" end
#endif // FLOAT_TYPE

// Using doubles
#ifdef D_TYPE
#define DataType double
#define DataSet fDataset
#define sgdX sgdXDataset
#define sgdY sgdYDataset
#define DataFormat(start, end) start "%3.6lf" end
#endif // DOUBLE_TYPE

#ifndef DataType
#error *Did not pass a type of value B_TYPE, H_TYPE, I_TYPE, F_TYPE or D_TYPE. Please define one of these.*
#endif // DataType

void initArray(DataType *array, int N);
void initArray2D(DataType *array, int N, int M);

void initZero(DataType *array, int N);
void initConstant(DataType *array, int N, DataType m);

#endif // FUNCTIONS_H
