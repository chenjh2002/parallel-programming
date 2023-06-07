#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include<pthread.h>
#include<omp.h>

void chunked_mm(int chunk, int n_chunks,int N,float** matrix_a,float** matrix_b,float** result_matrix) {
    for (int i = chunk*(N/n_chunks); i < (chunk+1)*(N/n_chunks); i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

int main(int argc,char* argv[]){
    if(argc<=2){
        printf("Please input the demension of matrix\n");
        exit(0);
    }
    
    int row=500,col=500;
    row=atoi(argv[1]);
    col=atoi(argv[2]);
    printf("row=%s,colunmn=%s\n",argv[1],argv[2]);

    float **mat1,**mat2,**result;

    mat1=(float**)malloc(sizeof(float*)*row);
    mat2=(float**)malloc(sizeof(float*)*col);
    result=(float**)malloc(sizeof(float*)*row);

    for(int i=0;i<row;i++){
        mat1[i]=(float*)malloc(sizeof(float)*col);
        for(int j=0;j<col;j++){
            mat1[i][j]=(float)rand()/(float)(RAND_MAX/10000);

        }
    }

    for(int i=0;i<col;i++){
        mat2[i]=(float*)malloc(sizeof(float)*row);
        for(int j=0;j<row;j++){
            mat2[i][j]=(float)rand()/(float)(RAND_MAX/10000);

        }
    }
    // printf("finish initialization.\n");
    // present the mat
    // for(int i=0;i<row;i++){
    //     for(int j=0;j<col;j++){
    //         printf("%lf ",mat1[i][j]);
    //     }
    //     printf("\n");
    // }

    for(int i=0;i<row;i++){
        result[i]=(float*)malloc(sizeof(float)*row);
        for(int j=0;j<row;j++){
            result[i][j]=0;
        }
    }

    // printf("%f\n",mat2[0][199]);

    struct timeval start,end;

    //begining
    gettimeofday(&start,NULL);

    #pragma omp parallel for
    for (int chunk = 0; chunk < 4; chunk++) {
        chunked_mm(chunk, 4,row,mat1,mat2,result);
    }
    // for(int i=0;i<row;i++){
    //     for(int j=0;j<row;j++){
    //         for(int k=0;k<col;k++){
    //             result[i][j]+=mat1[i][k]*mat2[k][j];
    //         }
    //     }
    // }
    gettimeofday(&end,NULL);
    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("time using:%lfs\n",time_using/1e6);

    for(int i=0;i<row;i++){
        free(mat1[i]);
        free(result[i]);
        mat1[i]=NULL;
        result[i]=NULL;
    }
    for(int i=0;i<col;i++){
        free(mat2[i]);
        mat2[i]=NULL;
    }

    free(mat1);
    free(mat2);
    free(result);

    mat1=NULL;
    mat2=NULL;
    result=NULL;

    return 0;
}