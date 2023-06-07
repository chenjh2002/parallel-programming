#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>

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


float** scatter_matrix(MPI_Comm comm,float** mat,int row,int col,int world_rank,int world_size){
    int num_rows_per_proc=row/world_size;
    float** part_mat=create_matrix(num_rows_per_proc,col,0);
    // for(int i=0;i<num_rows_per_proc;i++){
    //     for(int j=0;j<col;j++){
    //         printf("%f\t",part_mat[i][j]);
    //     }
    //     printf("\n");
    // }
    if(mat==NULL){
        MPI_Scatter(mat,num_rows_per_proc*col,MPI_FLOAT,&part_mat[0][0],
                num_rows_per_proc*col,MPI_FLOAT,0,comm);
    }else{
        MPI_Scatter(&mat[0][0],num_rows_per_proc*col,MPI_FLOAT,&part_mat[0][0],
                num_rows_per_proc*col,MPI_FLOAT,0,comm);
    }
    return part_mat;
}

void gather_matrix(MPI_Comm comm,float** part_result,float** result,
                int col,int num_rows_per_proc){
    if(result==NULL){
        MPI_Gather(&part_result[0][0],num_rows_per_proc*col,MPI_FLOAT,result,
                num_rows_per_proc*col,MPI_FLOAT,0,comm);
    }else{
        MPI_Gather(&part_result[0][0],num_rows_per_proc*col,MPI_FLOAT,&result[0][0],
                num_rows_per_proc*col,MPI_FLOAT,0,comm);
    }
}


int main(int argc,char **argv){
    //initialize the MPI environment
    MPI_Init(NULL,NULL);
    //find out rank,size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);

    if(world_rank==0){
        if(world_size<2){
            fprintf(stderr,"World size must greater than 1 for %s\n",argv[0]);
            MPI_Abort(MPI_COMM_WORLD,1);
        }

        //produce random matrix
        if(argc<4){
            fprintf(stderr,"Please input the complete size of mutiply matrix\n");
            MPI_Abort(MPI_COMM_WORLD,1);
        }
    }
    //set seed
    srand(time(NULL));

    int mat1_row=atoi(argv[1]),mat1_col=atoi(argv[2]);
    int mat2_row=atoi(argv[2]),mat2_col=atoi(argv[3]);
    //allocate the matrix to each process
    int num_row_per_proc=mat1_row/world_size,num_col_per_proc=mat2_col/world_size;
    float ** mat1=NULL,**mat2=NULL,**result=NULL;

    struct timeval start,end;

    if(world_rank==0){

        //initial the matrix
        mat1=create_matrix(mat1_row,mat1_col,1);
        mat2=create_matrix(mat2_row,mat2_col,1);
        result=create_matrix(mat1_row,mat2_col,0);
        //begining timer
        gettimeofday(&start,NULL);
    }else{
         mat2=create_matrix(mat2_row,mat2_col,0);
    }

    //send mat1,each row
    float** part_mat1=scatter_matrix(MPI_COMM_WORLD,mat1,mat1_row,mat1_col,world_rank,world_size);
    
    // printf("World Rank:%d,mat1:\n",world_rank);
    // for(int i=0;i<num_row_per_proc;i++){
    //     for(int j=0;j<mat1_col;j++){
    //         printf("%f\t",part_mat1[i][j]);
    //     }
    //     printf("\n");
    // }

    //send mat2
    MPI_Bcast(&mat2[0][0],mat2_row*mat2_col,MPI_FLOAT,0,MPI_COMM_WORLD);
    // printf("World Rank:%d,mat2:\n",world_rank);
    // for(int i=0;i<mat2_row;i++){
    //     for(int j=0;j<mat2_col;j++){
    //         printf("%f\t",mat2[i][j]);
    //     }
    //     printf("\n");
    // }
    //part result
    float** part_result=create_matrix(num_row_per_proc,mat2_col,0);

    int paramter[3]={num_row_per_proc,mat2_row,mat2_col};

    matmul(part_mat1,mat2,part_result,paramter);
    MPI_Barrier(MPI_COMM_WORLD);

    // printf("World Rank:%d,result mat:\n",world_rank);
    // for(int i=0;i<num_row_per_proc;i++){
    //     for(int j=0;j<mat2_col;j++){
    //         printf("%f\t",part_result[i][j]);
    //     }
    //     printf("\n");
    // }
    //gather
    gather_matrix(MPI_COMM_WORLD,part_result,result,mat2_col,
                num_row_per_proc);

    if(world_rank==0){
        gettimeofday(&end,NULL);
        double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
        printf("time using:%lfs\n",time_using/1e6);

        // printf("Result:\n");
        // for(int i=0;i<mat1_row;i++){
        //     for(int j=0;j<mat2_col;j++){
        //         printf("%f ",result[i][j]);
        //     }
        //     printf("\n");
        // }

        free_matrix(mat1);
        free_matrix(result);
    }


    free(part_mat1);
    free(part_result);
    free(mat2);

    MPI_Finalize();
    return 0;
}