#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
// #include"leak_detector_c.h"

void matmul(float** mat1,float** mat2,float** result,int *parameter){
    for(int i=0;i<parameter[0];i++){
        for(int j=0;j<parameter[2];j++){
            for(int k=0;k<parameter[1];k++){
                result[i][j]+=mat1[i][k]*mat2[k][j];
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

    printf("float:%ld bytes.\n",sizeof(float));
    printf("float*:%ld bytes.\n",sizeof(float*));

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

int main(){
    float** result=create_matrix(10,10,1);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            printf("%f\t",result[i][j]);
        }
        printf("\n");
    }
    free_matrix(result);
    // atexit(report_mem_leak);
}