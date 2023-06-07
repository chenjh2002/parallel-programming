#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define ROWS 1000
#define COLS 1000

void calculate_entropy(double *data, double *entropy, int rows, int cols) {
    #pragma acc data copyin(data[0:rows*cols]) copyout(entropy[0:cols])
    {
        #pragma accparallel loop collapse(2)
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                int index = j * cols + i;
                double value = data[index];
                entropy[i] -= value * log2(value);
            }
        }
    }
}

int main() {
    double *data = (double*) malloc(sizeof(double) * ROWS * COLS);
    double *entropy = (double*) malloc(sizeof(double) * COLS);

    // Initialize data
    for (int i = 0; i < ROWS * COLS; i++) {
        data[i] = (double) rand() / RAND_MAX;
    }

    // Calculate entropy
    calculate_entropy(data, entropy, ROWS, COLS);

    // Print results
    for (int i = 0; i < COLS; i++) {
        printf("Column %d entropy: %f\n", i, entropy[i]);
    }

    free(data);
    free(entropy);

    return 0;
