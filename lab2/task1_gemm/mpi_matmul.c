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
        mat1=(float**)malloc(sizeof(float*)*mat1_row);
        mat2=(float**)malloc(sizeof(float*)*mat2_row);
        result=(float**)malloc(sizeof(float*)*mat1_row);
    
        for(int i=0;i<mat1_row;i++){
            mat1[i]=(float*)malloc(sizeof(float)*mat1_col);
            for(int j=0;j<mat1_col;j++){
                mat1[i][j]=(float)rand()/(float)(RAND_MAX/10)-5;   
            }
        }

        for(int i=0;i<mat2_row;i++){
            mat2[i]=(float*)malloc(sizeof(float)*mat2_col);
            for(int j=0;j<mat2_col;j++){
                mat2[i][j]=rand()/(float)(RAND_MAX/10)-5;
            }
        }

        for(int i=0;i<mat1_row;i++){
            result[i]=(float*)malloc(sizeof(float)*mat2_col);
            for(int j=0;j<mat2_col;j++){
                result[i][j]=0;
            }
        }
        

        //begining timer
        gettimeofday(&start,NULL);

        //send mat1,each row
        for(int process=0;process<world_size;process++){
            for(int i=0;i<num_row_per_proc;i++){
                MPI_Send(&mat1[process*num_row_per_proc+i][0],mat1_col,MPI_FLOAT,
                    process,i,MPI_COMM_WORLD);
            }
        }

        //send mat2
        for(int process=0;process<world_size;process++){
            for(int i=0;i<mat2_row;i++){
                MPI_Send(&mat2[i][0],mat2_col,MPI_FLOAT,
                    process,i,MPI_COMM_WORLD);
            }
            // printf("mat2_row:%d mat2_col:%d\n",mat2_row,mat2_col);
        }

        //send result
        // for(int process=0;process<world_size;process++){
        //     MPI_Send(&result[process*num_row_per_proc][0],num_row_per_proc*mat2_col,MPI_FLOAT,
        //             process,0,MPI_COMM_WORLD);
        // }

        //send paramter of the data
        int paramter[3]={num_row_per_proc,mat2_row,mat2_col};
        for(int process=0;process<world_size;process++){
            MPI_Send(paramter,3,MPI_FLOAT,
                    process,0,MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // receive data from root
    float **part_mat1=NULL,**part_mat2=NULL,**part_result=NULL;
    int parameter[3];


    part_mat1=(float**)malloc(sizeof(float*)*num_row_per_proc);
    part_mat2=(float**)malloc(sizeof(float*)*mat2_row);
    part_result=(float**)malloc(sizeof(float*)*num_row_per_proc);

    for(int i=0;i<num_row_per_proc;i++){
        part_mat1[i]=(float*)malloc(sizeof(float)*mat1_col);
        part_result[i]=(float*)malloc(sizeof(float)*mat2_col);
    }

    for(int i=0;i<mat2_row;i++){
        part_mat2[i]=(float*)malloc(sizeof(float)*mat2_col);
    }
    // initial the part result
    for(int i=0;i<num_row_per_proc;i++){
        for(int j=0;j<mat2_col;j++){
            part_result[i][j]=0;
        }
    }

    MPI_Status status;
    int number_amount;

    for(int i=0;i<num_row_per_proc;i++){
        MPI_Recv(&part_mat1[i][0],mat1_col,MPI_FLOAT,
            0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    for(int i=0;i<mat2_row;i++){
        MPI_Recv(&part_mat2[i][0],mat2_col,MPI_FLOAT,
            0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    // MPI_Get_count(&status, MPI_FLOAT, &number_amount);
    // printf("amount:%d\n",number_amount);   
    // MPI_Recv(&part_result[0][0],num_row_per_proc*mat2_col,MPI_FLOAT,
    //         0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Recv(parameter,3,MPI_INT,
            0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    
    // for(int i=0;i<mat2_row;i++){
    //     for(int j=0;j<mat2_col;j++){
    //         printf("%f ",part_mat2[i][j]);
    //     }
    //     printf("\n");
    // }
    matmul(part_mat1,part_mat2,part_result,parameter);

    // printf("process id:%d\n",world_rank);
    // printf("mat1:\n");
    // for(int i=0;i<num_row_per_proc;i++){
    //     for(int j=0;j<mat1_col;j++){
    //         printf("%f ",part_mat1[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("mat2:\n");
    // for(int i=0;i<mat2_row;i++){
    //     for(int j=0;j<mat2_col;j++){
    //         printf("%f ",part_mat2[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("part_result:\n");
    // for(int i=0;i<num_row_per_proc;i++){
    //     for(int j=0;j<mat2_col;j++){
    //         printf("%f ",part_result[i][j]);
    //     }
    //     printf("\n");
    // }

    //gather data to root
    for(int i=0;i<num_row_per_proc;i++){
        MPI_Send(&part_result[i][0],mat2_col,MPI_FLOAT,0,i,MPI_COMM_WORLD);
    }
    // printf("process %d send part result to root\n",world_rank);

    MPI_Barrier(MPI_COMM_WORLD);


    if(world_rank==0){
        for(int process=0;process<world_size;process++){
            for(int i=0;i<num_col_per_proc;i++){
                MPI_Recv(&result[process*num_row_per_proc+i][0],num_row_per_proc*mat2_col,
                    MPI_FLOAT,process,i,MPI_COMM_WORLD,&status);
            }

            // MPI_Get_count(&status, MPI_FLOAT, &number_amount);
            // printf("result:%f,process:%d,amount:%d\n",result[process*num_row_per_proc][0],process,number_amount);
        }
        gettimeofday(&end,NULL);
        double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
        printf("time using:%lfs\n",time_using/1e6);
        // for(int i=0;i<mat1_row;i++){
        //     for(int j=0;j<mat2_col;j++){
        //         printf("%f ",result[i][j]);
        //     }
        //     printf("\n");
        // }


        for(int i=0;i<mat1_row;i++){
            free(mat1[i]);
            free(result[i]);
            mat1[i]=NULL;
            result[i]=NULL;
        }
        for(int i=0;i<mat2_row;i++){
            free(mat2[i]);
            mat2[i]=NULL;
        }

        free(mat1);
        free(mat2);
        free(result);

        mat1=NULL;
        mat2=NULL;
        result=NULL;
    }

    //free the local resource
    for(int i=0;i<num_row_per_proc;i++){
        free(part_mat1[i]);
        free(part_result[i]);
        part_mat1[i]=NULL;
        part_result[i]=NULL;
    }

    for(int i=0;i<mat2_row;i++){
        free(part_mat2[i]);
        part_mat2[i]=NULL;
    }

    free(part_mat1);
    free(part_mat2);
    free(part_result);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}