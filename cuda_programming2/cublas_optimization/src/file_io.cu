#include "file_io.hh"

using namespace std;

void readFile(const char* filePath, float *&mat, float *&vec ,int &row,int &col){
    std::ifstream inFile(filePath, std::ios::binary);
    std::vector<float> matrixInput;
    std::vector<float> vecInput;

    if(inFile.is_open()){
        inFile.read(reinterpret_cast<char *>(&row), sizeof(int));
        inFile.read(reinterpret_cast<char *>(&col), sizeof(int));

        matrixInput.resize(row * col);
        vecInput.resize(col);

        // read the input
        inFile.read(reinterpret_cast<char *>(matrixInput.data()), matrixInput.size() * sizeof(float));
        inFile.read(reinterpret_cast<char *>(vecInput.data()), vecInput.size() * sizeof(float));

        inFile.close();

        // translate it into array form
        mat = globalAllocaMat1Dim(row, col, false);
        vec = globalAllocaVec(col, false);


        for(int i = 0; i < row * col; i++){
            mat[i] = matrixInput[i];
            // cout<<mat[i]<<"\t";
        }

        for(int i = 0; i < col; i++){
            vec[i] = vecInput[i];
            // cout<<"1\n";
        }

    }else{
        std::cout << "Unable to open file\n";
    }
}

void testFile(const char* filePath, float* res,int row){
    std::ofstream outFile_out(filePath, std::ios::binary);
    if (outFile_out.is_open()) {
        vector<float> result(res, res + row);
        // for(int i = 0;i < row ; i++){
        //     cout<<result[i]<<"\t";
        // }
        outFile_out.write(reinterpret_cast<const char *>
        (result.data()), result.size() * sizeof(float));
        outFile_out.close();

    } else {
        std::cout << "Unable to open file";
    }   
}

// int main(int argc, char* argv[]){
//     float* mat=NULL, *vec=NULL;
//     int row , col;
//     readFile("../validation_file/test1.in", &mat, &vec, row, col);

//     for(int i = 0; i< row * col; i++){
//         cout<<mat[i]<<"\t";
//     }

//     return 0;
// }