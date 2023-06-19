#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string.h>
#include <stdlib.h>

using namespace std;

void readSize(const char* filePath, int &row,int &col){
    std::ifstream inFile(filePath, std::ios::binary);

    if(inFile.is_open()){
        std::cout << "open file "<<filePath<<" successsed\n";
        inFile.read(reinterpret_cast<char *>(&row), sizeof(int));
        inFile.read(reinterpret_cast<char *>(&col), sizeof(int));
        inFile.close();
        return;
    }else{
        std::cout << "Unable to open file"<<filePath<<"\n";
    }
}

vector<float> readResult(const char* filePath, const int &length){
    std::ifstream inFile(filePath, std::ios::binary);
    vector<float> vec;
    vec.resize(length);

    if(inFile.is_open()){

        // read the input
        std::cout << "open file "<<filePath<<" successsed\n";
        inFile.read(reinterpret_cast<char *>(vec.data()), length * sizeof(float));
        inFile.close();

        return vec;
    }else{
        std::cout << "Unable to open file\n";
    }
}

int main(int argc, char* argv[]){
    int row , col;

    for(int round = 0; round < 5; round++){
        char standard_file_in[50], standard_file_out[50], my_file_path[50];
        sprintf(standard_file_in, "./test%d.in", round+1);
        sprintf(standard_file_out, "./test%d.out", round+1);
        sprintf(my_file_path, "./original_version/my_test%d.out", round+1);

        readSize(standard_file_in, row, col);
        vector<float> standardRes = readResult(standard_file_out, row);
        vector<float> myRes = readResult(my_file_path, row);
    
        for(int i = 0; i < row; i++){
            if(fabs(standardRes[i] - myRes[i]) > 1e-4){
                cout<<"error"<<endl;
                exit(1);
            }
        }

        cout<<"test "<<round+1<<" passed"<<endl;
    }
    return 0;
}