#include<iostream>
#include<vector>
#include<sys/time.h>
#include<cstring>

using namespace std;

int main(int argc,char* argv[]){
    if(argc<=2){
        printf("Please input the demension of matrix\n");
        exit(0);
    }

    vector<vector<float> > mat1,mat2,result;
    int row,col;
    row=atoi(argv[1]);
    col=atoi(argv[2]);
    printf("row=%s,colunmn=%s\n",argv[1],argv[2]);

    mat1.resize(row);
    mat2.resize(col);
    result.resize(row);

    // initialization
    for(int i=0;i<row;i++){
        mat1[i].resize(col);
        for(int j=0;j<col;j++){
            mat1[i][j]=(float)rand()/(float)(RAND_MAX/10000);
        }
    }

    for(int i=0;i<col;i++){
        mat2[i].resize(row);
        for(int j=0;j<col;j++){
            mat2[i][j]=(float)rand()/(float)(RAND_MAX/10000);
        }
    }

    for(int i=0;i<row;i++){
        result[i].resize(row);
        for(int j=0;j<row;j++){
            result[i][j]=0;
        }
    }

    struct timeval start,end;

    //begining
    gettimeofday(&start,NULL);
    for(int i=0;i<row;i++){
        for(int j=0;j<row;j++){
            for(int k=0;k<col;k++){
                result[i][j]+=mat1[i][k]*mat2[k][j];
            }
        }
    }
    gettimeofday(&end,NULL);
    double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    printf("time using:%lfs\n",time_using/1e6);

    return 0;
}