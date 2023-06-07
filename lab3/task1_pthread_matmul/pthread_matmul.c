#include<stdio.h>
#include<sys/time.h>
#include<stdint.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

#define DIM 256

#define NUM_THREADS 16

float matrix_a[DIM][DIM];
float matrix_b[DIM][DIM];
float matrix_c[DIM][DIM];

typedef struct{
    int start;
    int end;
}thread_args;

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void init(){
    for(int i=0;i<DIM;i++){
        for(int j=0;j<DIM;j++){
            matrix_a[i][j]=rand()/(float)(RAND_MAX/10)-5;
            matrix_b[i][j]=rand()/(float)(RAND_MAX/10)-5;
        }
    }
}

void *multiply(void * args){
    thread_args* range=(thread_args*)args;
    for(int i=0;i<DIM;i++){
        for(int j=0;j<DIM;j++){
            float thread_private_tmp=0;
            for(int k=range->start;k<range->end;k++){
                thread_private_tmp+=matrix_a[i][k]*matrix_b[k][j];
            }
            pthread_mutex_lock(&lock);
            matrix_c[i][j]+=thread_private_tmp;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

// redirection
void print(){
    FILE* fp=fopen("parallel.txt","w");
    for(int i=0;i<DIM;i++){
        for(int j=0;j<DIM;j++){
            fprintf(fp,"%.2f\t",matrix_c[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

int main(void){
    pthread_t child_threads[NUM_THREADS];
    thread_args work_range[NUM_THREADS];
    struct timeval start,end;
    int current_start,range;

    //compute the range
    current_start=0;
    range=DIM/NUM_THREADS;
    
    for(int i=0;i<NUM_THREADS;i++){
        work_range[i].start=current_start;
        work_range[i].end=current_start+range;
        current_start+=range;
    }

    //if cnannot slash completely
    work_range[NUM_THREADS-1].end=DIM;

    //inittialize the matrix
    init();

    //start timer
    gettimeofday(&start,NULL);

    //task start
    for(int i=0;i<NUM_THREADS;i++){
        pthread_create(&child_threads[i],NULL,multiply,&work_range[i]);
    }

    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(child_threads[i],NULL);
    }

    //end timer
    gettimeofday(&end,NULL);
    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
        printf("time using:%lfs\n",time_using/1e6);
    //redirection the result to file.
    print();
    return 0;
}