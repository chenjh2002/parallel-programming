#ifndef MATRIX_VECTOR_MULTIPLY_HH
#define MATRIX_VECTOR_MULTIPLY_HH

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
#include<fcntl.h>
#include<sys/time.h>
#include<cuda_runtime.h>

__global__ void matrixVectorMutiply1Dim(float *A,float *b, float *c,int row,int col);

void matrixVectorMutiplyExecute1Dim(float* mat, float* vec, float* res, int row, int col,int blockSize);

float* globalAllocaMat1Dim(int row, int col, bool isRand);
// float* globalAllocaMat2Dim(int row, int col, bool isRand = false);
float* globalAllocaVec(int length, bool isRand);

void produceRandomArray(float* mat,int size);
void globalFree(float* mat,float *vec, float *result);

#endif
