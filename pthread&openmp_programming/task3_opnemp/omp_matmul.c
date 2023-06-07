#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<omp.h>

typedef struct{
    int start;
    int end;
}allocation;

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

int main(int argc,char* argv[]){
    if(argc<4){
        fprintf(stderr,"please input the size of matrix\n");
    }

    int mat1_row=atoi(argv[1]),mat1_col=atoi(argv[2]);
    int mat2_row=atoi(argv[2]),mat2_col=atoi(argv[3]);


    float **mat1,**mat2,**result;

    //initialize the matrix
    mat1=create_matrix(mat1_row,mat1_col,1);
    mat2=create_matrix(mat2_row,mat2_col,1);
    result=create_matrix(mat1_row,mat2_col,0);

    struct timeval start,end;

    //start timer
    gettimeofday(&start,NULL);

    omp_set_num_threads(16);
    allocation allocate[16];

    #pragma omp parallel shared(allocate)
    {   
        int thread_id=omp_get_thread_num();
        int total_thread=omp_get_num_threads();
        // printf("total thread number: %d\n",total_thread);
        
        int current_start=0,range=mat1_row/total_thread;

        #pragma omp master
        for(int i=0;i<total_thread;i++){
            allocate[i].start=current_start;
            allocate[i].end=current_start+range;
            current_start+=range;
            if(i==total_thread-1){
                allocate[i].end=mat1_row;
            }
        }

        #pragma omp barrier

        float tmp=0;
        // printf("thread_id: %d, start: %d, end:%d\n",thread_id,allocate[thread_id].start,allocate[thread_id].end);
        // #pragma omp parallel for firstprivate(tmp) collapse(2)
        for(int i=0;i<mat1_row;i++){
            for(int j=0;j<mat2_col;j++){
                for(int k=allocate[thread_id].start;k<allocate[thread_id].end;k++){
                    tmp+=mat1[i][k]*mat2[k][j];
                }
                #pragma omp critical
                result[i][j]+=tmp; 
            }
        }
    }

    //end timer
    gettimeofday(&end,NULL);
    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("time using:%lfs\n",time_using/1e6);

    //free the source
    free(mat1);
    free(mat2);
    free(result);

    return 0;
}