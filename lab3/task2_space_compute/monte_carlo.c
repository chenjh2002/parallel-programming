#include<math.h>
#include<pthread.h>

#include<stdio.h>
#include<stdlib.h>

#include<time.h>
#include<sys/time.h>


int total_experiment=1e8,num_threads=8,result;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

typedef struct 
{
   int experiment;
}pthread_struct;


void *worker(void* args){
    pthread_struct* size=(pthread_struct*)args;
    int tmp_result=0;
    for(int i=0;i<size->experiment;i++){
        double x_random=rand()/(double)RAND_MAX;
        double y_random=rand()/(double)RAND_MAX;
        if(pow(x_random,2)>=y_random){
            tmp_result+=1;
        }
    }

    pthread_mutex_lock(&mutex);
    result+=tmp_result;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc,char* argv[]){
    if(argc<3){
        fprintf(stderr,"please input the time of experience and number of threads");
    }

    total_experiment=atoi(argv[1]);
    num_threads=atoi(argv[2]);

    pthread_t threads[num_threads];
    pthread_struct works[num_threads];
    struct timeval start,end;

    for(int i=0;i<num_threads;i++){
        works[i].experiment=total_experiment/num_threads;
    }
    works[num_threads-1].experiment+=total_experiment%num_threads;

    gettimeofday(&start,NULL);
    for(int i=0;i<num_threads;i++){
        pthread_create(&threads[i],NULL,worker,&works[i]);
    }

    for(int i=0;i<num_threads;i++){
        pthread_join(threads[i],NULL);
    }

    printf("result of x^2 space: %.10f.\n",(float)result/(float)total_experiment);
    //end timer
    gettimeofday(&end,NULL);

    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("time using:%lfs\n",time_using/1e6);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}