#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/time.h>
#include<cuda_runtime.h>
#define BLOCK_SIZE 32


__global__ void entropy_kernel(int* input,float* output,float *logVal,int width,int height,int window_size){
    __shared__ int shared_mem[122];

    if(threadIdx.x==0){
        for(int i=0;i<122;i++){
            shared_mem[i]=logVal[i];
        }
    }

    __syncthreads();

    int x=blockIdx.x*blockDim.x+threadIdx.x;
    int y=blockIdx.y*blockDim.y+threadIdx.y;

    if(x<width && y<height){
        float entropy=0.0;

        int p[15]={0};
        for(int i=-window_size/2;i<window_size/2;i++){
            for(int j=-window_size/2;j<window_size/2;j++){
                int nx=x+i;
                int ny=y+j;

                if(nx>=0 && nx<width && ny>=0 && ny<height){
                    // use the value stored
                    p[input[ny*width+nx]]+=1;
                } 
            }
        }

        for(int i=0;i<15;i++){
            if(p[i]==0) continue;
            entropy+=(float)p[i]/(window_size*window_size)*(shared_mem[p[i]]-shared_mem[window_size*window_size]);
        }

        output[y*width+x]=-entropy;
    }
}

void allocate(int *array,float *entropy,int width,int height,int window_size){
    // value pass to GPU memory
    int *distributedArray=NULL;
    float *distributedLogVal=NULL,*logVal=NULL;
    float* distributedEntropyArray=NULL;

    // calculate size
    int arraySize=width*height*sizeof(int);
    int entropySize=width*height*sizeof(float);
    // max window size 121
    int logSize=122;

    // store the log value first
    logVal=(float*)malloc(sizeof(float)*logSize);

    for(int i=0;i<logSize;i++){
        if(i==0){
            logVal[0]=-9999;
        }else{
            logVal[i]=log2((float)i);
        }
        // printf("%.2f ",logVal[i]);
    }

    // ready the value needed
    cudaMalloc(&distributedArray,arraySize);
    cudaMalloc(&distributedEntropyArray,entropySize);
    cudaMalloc(&distributedLogVal,logSize*sizeof(float));

    
    cudaMemcpy(distributedArray,array,arraySize,cudaMemcpyHostToDevice);
    cudaMemcpy(distributedEntropyArray,entropy,entropySize,cudaMemcpyHostToDevice);
    cudaMemcpy(distributedLogVal,logVal,logSize*sizeof(float),cudaMemcpyHostToDevice);

    // launch the kernel
    dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
    dim3 dimGrid((width+dimBlock.x-1)/dimBlock.x,(height+dimBlock.y-1)/dimBlock.y);

    entropy_kernel<<<dimGrid,dimBlock>>>(distributedArray,distributedEntropyArray,distributedLogVal,
        width,height,window_size);
    
    cudaMemcpy(entropy,distributedEntropyArray,entropySize,cudaMemcpyDeviceToHost);


    // free memory
    cudaFree(distributedArray);
    cudaFree(distributedEntropyArray);
    cudaFree(distributedLogVal);

    free(logVal);

    return;
}

int main(int argc,char* argv[]){
    if(argc<4){
        fprintf(stderr,"Please input the size of the 2-dimension array.\n");
        exit(1);
    }

    int width=atoi(argv[1]),height=atoi(argv[2]),window_size=atoi(argv[3]);
    // printf("%d %d %d\n",width,height,window_size);

    int *array=(int*)malloc(sizeof(int)*width*height);
    float* entropy=(float*)malloc(sizeof(float)*width*height);

    // initial value between [0-15]
    srand(time(NULL));
    for(int i=0;i<width*height;i++){
        array[i]=(int)(rand()/(float)RAND_MAX*15);
        entropy[i]=0;
    }

    // // define the file 
    // int fd;
    // char INPUTFILE[]="random_input.txt";
    // char OUTPUTFILE[]="entropy.txt";

    // // store input
    // fd=open(INPUTFILE,O_CREAT|O_WRONLY,0644);
    // if(fd<0){
    //     fprintf(stderr,"open input file error");
    //     exit(1);
    // }

    // for(int i=0;i<width*height;i++){
    //     char buffer[10];
    //     sprintf(buffer,"%d\t",array[i]);
    //     write(fd,buffer,strlen(buffer));

    //     if((i+1) % height==0){
    //         write(fd,"\n",1);
    //     }
    // }

    // close(fd);

    struct timeval start,end;
    //begining timer
    gettimeofday(&start,NULL);

    // calculate
    allocate(array,entropy,width,height,window_size);

    // end timer;
    gettimeofday(&end,NULL);
    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("time using:%lfs\n",time_using/1e6);

    // // store output
    // fd=open(OUTPUTFILE,O_CREAT|O_WRONLY,0644);
    // if(fd<0){
    //     fprintf(stderr,"open input file error");
    //     exit(1);
    // }

    // for(int i=0;i<width*height;i++){
    //     char buffer[10];
    //     sprintf(buffer,"%.2f\t",entropy[i]);
    //     write(fd,buffer,strlen(buffer));

    //     if((i+1) % height==0){
    //         write(fd,"\n",1);
    //     }
    // }

    // close(fd);

    // free memory
    free(array);
    free(entropy);

    return 0;
}