#include"lib/matmul.hpp"
#include<iostream>
#include<cstdlib>

using namespace std;

int main(int argc,char** argv){
    if(argc<4){
        fprintf(stderr,"need to input the complete size of the matrix.\n");
        exit(1);
    }

    int mat1_row=atoi(argv[1]),mat1_col=atoi(argv[2]);
    int mat2_row=atoi(argv[2]),mat2_col=atoi(argv[3]);

    // printf("%d %d %d\n",mat1_row,mat1_col,mat2_col);
    matrix mat1,mat2,result;

    mat1.row=mat1_row;
    mat1.col=mat1_col;

    mat2.row=mat2_row;
    mat2.col=mat2_col;

    result.row=mat1_row;
    result.col=mat2_col;

    mat1.data=create_matrix(mat1.row,mat1.col,1);
    mat2.data=create_matrix(mat2.row,mat2.col,1);
    result.data=create_matrix(result.row,result.col,0);

    matmul(mat1,mat2,result);
    
    for(int i=0;i<result.row;i++){
        for(int j=0;j<result.col;j++){
            printf("%f\t",result.data[i][j]);
        }
        printf("\n");
    }

    free_matrix(mat1.data);
    free_matrix(mat2.data);
    free_matrix(result.data);

    return 0;
}