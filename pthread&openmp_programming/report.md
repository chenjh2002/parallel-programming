# 并行计算第三次实验
学号:20337011 姓名:陈俊皓

## 实验要求
1. 通过`pthreads`实现通用矩阵乘法。
2. 编写一个多线程程序来实现面积计算。
3. 通过`OpenMP`实现通用矩阵乘法。
4. 基于`OpenMP`的通用矩阵乘法优化。

## 实验过程
### pthread通用矩阵乘法

#### 定义线程形参结构体
对于`pthread`线程而言，其形参为一个类型为`void`的指针。因此，我们需要定义传递给线程的结构体以更好发挥线程的性能。在实验一中，定义的结构体如下:
```C
typedef struct{
    int start;
    int end;
}thread_args;
```

该结构体的作用在于定义每一个线程分配的计算范围,该范围由矩阵大小及创建的线程个数限定。

### 线程任务函数
在完成结构体定义之后，可以设计线程的任务函数，其中使用`pthread_mutex_lock`及`pthread_mutex_unlock`实现临界区资源的保护。
```C
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
```

### 执行结果:
<table>
    <thead>
        <tr>
            <td rowspan=2>Comm_size(number of threads)
            </td>
            <td colspan=5>Order of Matrix(Speedups, seconds)
            </td>
        </tr>
        <tr>
            <td>256</td>
            <td>512</td>
            <td>1024</td>
            <td>2048</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>4</td>
            <td>0.062183</td>
            <td>0.242067</td>
            <td>1.820078</td>
            <td>4.151120</td>
        </tr>
        <tr>
            <td>8</td>
            <td>0.110838</td>
            <td>0.493607</td>
            <td>1.939890</td>
            <td>8.604185</td>
        </tr>
        <tr>
            <td>16</td>
            <td>0.215994</td>
            <td>0.951401</td>
            <td>4.187733</td>
            <td>15.278418</td>
        </tr>
    </tbody>
<table>

## 编写一个多线程程序来实现面积计算
面积计算的`monte-carlo`方法基本原理是在某个确定面积的区域内随机产生点，统计随机点在目标范围内的个数，通过`大数定理`的理论我们可以得到所求面积$S_{target}$可以用已知面积$s_{source}$表达如下:
$$
    s_{target}=\frac{points\_in\_taregt}{total\_points}s_{source}
$$

多线程实现上述过程的加速即通过多个线程同时实现上述过程的模拟。具体的任务函数如下:
```C
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
```

执行结果:
<table>
    <thead>
        <tr>
            <td>线程数</td>
            <td>模拟次数</td>
            <td>计算结果</td>
            <td>时间消耗</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=2>4</td>
            <td>1000000</td>
            <td>0.3326889873</td>
            <td>0.305081</td>
        </tr>
        <tr>
            <td>5000000</td>
            <td>0.3332833946</td>
            <td>1.656986</td>
        </tr>
        <tr>
            <td rowspan=2>8</td>
            <td>1000000</td>
            <td>0.3339999914</td>
            <td>0.564082</td>
        </tr>
        <tr>
            <td>5000000</td>
            <td>0.3333393931</td>
            <td>2.462995</td>
        </tr>
    </tbody>
</table>

## 通过`OpenMP`实现通用矩阵乘法
`OpenMP`是一种高效实现并行化的方法,通过`#pragma omp ...`的形式由系统自动实现并行化的过程。具体的事例如下:
```C
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
```
在矩阵乘法的场景下，我们可以通过分发数据的方法利用`OpenMP`进行加速，具体的实现如下:
```C
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
```

### 执行结果
```C
chenjh2002@LAPTOP-SUUQN6NE:~/并行计算/lab3/task3_opnemp$ ./omp_matmul 256 256 256
time using:0.277182s
chenjh2002@LAPTOP-SUUQN6NE:~/并行计算/lab3/task3_opnemp$ ./omp_matmul 512 512 512
time using:1.107704s
chenjh2002@LAPTOP-SUUQN6NE:~/并行计算/lab3/task3_opnemp$ ./omp_matmul 1024 1024 1024
time using:4.309952s
chenjh2002@LAPTOP-SUUQN6NE:~/并行计算/lab3/task3_opnemp$ ./omp_matmul 2048 2048 2048
time using:16.644933s
```

## 基于`OpenMP`的通用矩阵乘法优化
`OpenMP`有多种调度策略，包括默认模式，静态模式`static`以及动态模式`dynamic`。以下为实现不同模式的语句。
```C
// default
#pragma omp parallel for
// static
#pragma omp parallel for schedule(static,1)
// dynamic
#pragma omp parallel for schedule(dynamic,1)
```

### 执行结果
<table>
    <thead>
        <tr>
            <td rowspan=2>Schedule
            </td>
            <td colspan=5>Order of Matrix(Speedups, seconds)
            </td>
        </tr>
        <tr>
            <td>256</td>
            <td>512</td>
            <td>1024</td>
            <td>2048</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>defult</td>
            <td>0.017392</td>
            <td>0.115681</td>
            <td>2.304199</td>
            <td>28.326686</td>
        </tr>
        <tr>
            <td>static</td>
            <td>0.020752</td>
            <td>0.116204</td>
            <td>2.298010</td>
            <td>31.570526</td>
        </tr>
        <tr>
            <td>dynamic</td>
            <td>0.014928</td>
            <td>0.113375</td>
            <td>1.358896</td>
            <td>25.315247</td>
        </tr>
    </tbody>
<table>

## 实验心得
通过此次实验，我了解到`pthread`以及`OpenMP`的应用技巧。

