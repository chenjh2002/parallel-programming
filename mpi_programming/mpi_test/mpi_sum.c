#include<mpi.h>
#include<stdio.h>
#include<math.h>

#define MAX_SIZE 1000

void main(int argc,char* argv){
    int myid,numprocs;
    int data[MAX_SIZE],i,x,low,high,myresult,result;
    char fn[255];
    char *fp;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if(myid==0){
        strcpy(fn,getenv("HOME"));
        strcat(fn,"/MPI/rand_data.txt");
        if((fp=fopen(fn,"r"))==NULL){
            printf("Can't open the input file:%s\n\n",fn);
            exit(1);
        }
        for(i=0;i<MAX_SIZE;i++){
            fscanf(fp,"%d",&data[i]);
        }
    }
}