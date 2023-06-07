#include<omp.h>
#include<stdio.h>

int main(){
    omp_set_num_threads(4);
    #pragma omp parallel num_threads(16)
    {
        int ID=omp_get_thread_num();
        printf("hello world %d!\n",ID);
    }
}

/*
Count the thread
omp_get_thread_num() --get the id of current thread
omp_get_num_threads() --get total number of threads

Synchronization
#pragmae omp barrier --let every threads sychronize at the step
#pragma omp critical --only one threads at one time can enter the cs
#pragma omp parallel --parallel execution
#pragma omp atomic   --atomic execution

Nested loops
#pragma omp for      --the variance defined in for loop is private
#pragma omp parallel for collapse(2) --indicate the number of threads

Reduction
#pragma omp parallel for reduction(+:ave)

reduction(op:list)  the varibales in "list" must be shared

Matser Contruct
#progma omp master --master thread do something

*/