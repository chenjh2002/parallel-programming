#include<stdio.h>
#include"matmul.hpp"
#include<stdlib.h>
#include<time.h>
#include<assert.h>

void matmul(matrix mat1,matrix mat2,matrix result){
    // assert(mat1.col!=mat2.row||result.row!=mat1.row||result.col!=mat2.col);

    for(int i=0;i<mat1.row;i++){
        for(int j=0;j<mat2.col;j++){
            for(int k=0;k<mat1.col;k++){
                result.data[i][j]+=mat1.data[i][k]*mat2.data[k][j];
            }
        }
    }
}

void initial_matrix(float** result,int row,int col,int random){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(random==1){
                result[i][j]=rand()/(float)(RAND_MAX/10)-5;
            }else{
                result[i][j]=0;
            }
        }
    }
    return;
}

float** create_matrix(int row,int col,int random){
    float **result=NULL;
    float *one_dim=NULL;

    result=(float**)malloc((sizeof(float*)*row));
    one_dim=(float*)malloc(sizeof(float)*row*col);

    for(int i=0;i<row;i++){
        result[i]=&one_dim[i*col];
    }

    initial_matrix(result,row,col,random);

    return result;
}

void free_matrix(float** result){
    free(*result);
    free(result);
    result=NULL;
}
