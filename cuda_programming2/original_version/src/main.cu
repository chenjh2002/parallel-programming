#include "file_io.hh"
#include "mat_vec_multiply.hh"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream> 
#include <algorithm>


using namespace std;

string helpTxt = \
"help:  [name]                  [option...]\n"
"       -m  --mode              test/random\n"
"       -t  --test-case         1...5\n"
"       --time                   true/false\n"
"       -b  --block-size        set block size\n"
"       -i  --input-size        set input-size\n";

class InputParser{
public:
    InputParser (int &argc, char **argv){
        for (int i=1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }

    const std::string& getCmdOption(const std::string &option) const{
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool cmdOptionExists(const std::string &option) const{
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }
private:
    std::vector <std::string> tokens;
};

int main(int argc, char* argv[]){
    InputParser input(argc, argv);

    if(input.cmdOptionExists("-h") || input.cmdOptionExists("--help")){
        cout<<helpTxt;
        exit(1);
    }

    // initial data
    string mode;
    if(input.cmdOptionExists("-m") || input.cmdOptionExists("--mode")){
        if(input.cmdOptionExists("-m"))
            mode = input.getCmdOption("-m");
        else
            mode = input.getCmdOption("--mode");
    }else{
        fprintf(stderr, "need the mode arment");
    }

    float* mat = NULL, *vec = NULL, *res = NULL;
    int row=256 , col=256;

    string fileInPath,fileOutPath;
    if(mode == "test"){
        // test mode
        if(input.cmdOptionExists("-t") || input.cmdOptionExists("--test-case")){
            if(input.cmdOptionExists("-t")){
                fileInPath = "../validation_file/test"+input.getCmdOption("-t")+".in";
                fileOutPath = "../validation_file/my_test"+input.getCmdOption("-t")+".out";
            }
            else{
                fileInPath = "../validation_file/test"+input.getCmdOption("--test-case")+".in";
                fileOutPath = "../validation_file/my_test"+input.getCmdOption("--test-case")+".out";
            }
        }

        // cout<<fileInPath<<endl;
        // load data
        readFile(fileInPath.data(),mat, vec, row, col);
        res = globalAllocaVec(row, true);
    }else{
        if(input.cmdOptionExists("-i") || input.cmdOptionExists("--input-size")){
            if(input.cmdOptionExists("-i")){
                row = atoi(input.getCmdOption("-i").c_str());
                col = atoi(input.getCmdOption("-i").c_str());
            }else{
                row = atoi(input.getCmdOption("--input-size").c_str());
                col = atoi(input.getCmdOption("--input-size").c_str());
            }
        }

        mat = globalAllocaMat1Dim(row, col, true);
        vec = globalAllocaVec(col, true);
        res = globalAllocaVec(row, true);
    }


    // set default block value
    int blockSize = 32;
    if(input.cmdOptionExists("-b") || input.cmdOptionExists("--block-size")){
        if(input.cmdOptionExists("-b"))
            blockSize = atoi(input.getCmdOption("-b").c_str());
        else
            blockSize = atoi(input.getCmdOption("--block-size").c_str());
    }

    // timer setting
    struct timeval start, end;
    if(input.cmdOptionExists("--time")){
        gettimeofday(&start, NULL);
    }

    // launch computing
    matrixVectorMutiplyExecute1Dim(mat, vec, res, row, col, blockSize);

    if(input.cmdOptionExists("--time")){
        gettimeofday(&end,NULL);
        double time_using=1e6*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
        printf("time using:%lfs\n",time_using/1e6);
    }

    // free the memory
    if(mode == "random"){
        globalFree(mat, vec, res);
    }else{
        testFile(fileOutPath.data(), res, row);
        globalFree(mat, vec, res);
    }

    return 0;
}