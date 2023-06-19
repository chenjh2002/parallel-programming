#include "mat_vec_multiply.hh"
#include <cublas_v2.h>

// #define BLOCK_SIZE 32

__global__ void matrixVectorMutiply1Dim(float *A, float *b, float *c,int row,int col){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < row){
        float sum = 0.0;
        for(int j =0; j < col; j++){
            sum += A[i * col + j] * b[j];
        }
        c[i] = sum;
    }
}

// __global__ void matrixVectorMutiply2Dim(float** A, float *b, float* c, int row, int col){
//     int x = blockIdx.x * blockDim.x + threadIdx.x;

// }

void matrixVectorMutiplyExecute1Dim(float* mat, float* vec, float* res, int row, int col,int blockSize){
    // value pass to GPU memory
    float *distributedMat = NULL;
    float *distributedVec = NULL;
    float *distributedRes = NULL;

    // calculate szie
    int matSize = row * col *sizeof(float);
    int vecSize = col * sizeof(float);
    int resSize = row * sizeof(float);

    // prepare the value
    cudaMalloc(&distributedMat, matSize);
    cudaMalloc(&distributedVec, vecSize);
    cudaMalloc(&distributedRes, resSize);

    cudaMemcpy(distributedMat, mat, matSize, cudaMemcpyHostToDevice);
    cudaMemcpy(distributedVec, vec, vecSize, cudaMemcpyHostToDevice);
    cudaMemcpy(distributedRes, res, resSize, cudaMemcpyHostToDevice);

    // // launch the kernel
    // dim3 dimBlock(blockSize);
    // dim3 dimGrid((row + blockSize - 1) / blockSize);

    // matrixVectorMutiply1Dim<<<dimBlock,dimGrid>>>(distributedMat, distributedVec, distributedRes, row, col);

    cublasHandle_t handle;
    cublasCreate(&handle);
    float alpha = 1.0f, beta = 0.0f;

    cublasSgemv(handle, CUBLAS_OP_N, row, col, &alpha, distributedMat, row, distributedVec, col, &beta, distributedRes, 1);

    // remove data from device to host
    cudaMemcpy(res,distributedRes,resSize,cudaMemcpyDeviceToHost);

    // free memory
    cudaFree(distributedMat);
    cudaFree(distributedVec);
    cudaFree(distributedRes);

    return;
}

void produceRandomArray(float* mat,int size){
    for(int i=0; i < size; i++){
        mat[i]=rand() / (float)RAND_MAX * 125; 
    }
}

float* globalAllocaMat1Dim(int row, int col, bool isRand){
    float* mat = (float*)malloc(row * col * sizeof(float));
    int matSize = row * col;

    if(isRand){
        produceRandomArray(mat, matSize);
    }else{
        memset(mat, 0, matSize * sizeof(float));
    }

    return mat;
}

float** globalAllocaMat2Dim(int row, int col, bool isRand){
    float* mat = (float*)malloc(row * col * sizeof(float));
    int matSize = row * col;

    if(isRand){
        produceRandomArray(mat, matSize);
    }else{
        memset(mat, 0, matSize * sizeof(float));
    }

    float** mat2Dim = (float**)malloc(row * sizeof(float*));
    for(int i=0; i < row; i++){
        mat2Dim[i] = &mat[i * (int)col];
    }

    return mat2Dim;
}

float* globalAllocaVec(int length, bool isRand){
    float* vec = (float*)malloc(length * sizeof(float));
    if(isRand){
        produceRandomArray(vec, length);
    }else{
        memset(vec, 0, length * sizeof(float));
    }

    return vec;
}


void globalFree(float* mat,float *vec, float *result){
    free(mat);
    free(vec);
    free(result);

    return;
}

// int main(int argc, char * argv[]){
//     int row =256, col = 256;
//     float * mat, * vec, *res;

//     mat = globalAllocaMat1Dim(row, col, true);
//     vec = globalAllocaVec(col, true);
//     res = globalAllocaVec(row, false);

//     matrixVectorMutiplyExecute1Dim(mat, vec, res, row, col,32);

//     for(int i = 0; i < row; i++){
//         printf("%.5f\t",res[i]);
//     }
//     printf("\n");

//     globalFree(mat, vec, res);

//     return 0;
// }
