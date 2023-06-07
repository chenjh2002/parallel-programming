#include<omp.h>
#include<stdio.h>

int main(){
    #pragma omp parallel for schedule(dynamic,1)
    for(int i=0;i<20;i++){
        printf("Thread %d is runnning  number %d\n",omp_get_thread_num(),i);
    }

    return 0;
}