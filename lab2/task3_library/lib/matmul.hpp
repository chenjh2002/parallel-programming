#ifndef LIB1_H_INCLUDED
#define LIB1_H_INCLUDED

#ifdef _cpluscplus
    extern "C"{
#endif

typedef struct
{
    float** data;
    int row;
    int col;
}matrix;

void matmul(matrix mat1,matrix mat2,matrix result);

float** create_matrix(int row,int col,int random);

void free_matrix(float** result);

#ifdef _cpluscplus
    }
#endif

#endif