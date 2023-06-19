#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include "mat_vec_multiply_texture.hh"

using namespace std;

void readFile(const char* filePath, float *&mat, float *&vec, int &row, int &col);
void testFile(const char* filePath, float *res,int row);