// saxypy program ax + y -> y
#include <stdlib.h>
#include <stdio.h>

void printNVectors(float ** vectors, char * vectorNames, int n, int c_n) {
    for (int i = 0; i < n; ++i) {
        printf("Contents of vector %c: (", vectorNames[i]);
        for (int j = 0; j < c_n; ++j) printf("%f,", vectors[i][j]);
        printf(")\n");
    }
}

void saxpy(float a, float * x, float * y, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        printf("Thread %d running :)\n\tCalculating y[%d] = (%f * %f) + %f\n", i, i, x[i], a, y[i]);
        y[i] = (x[i]*a) + y[i];
    }
}

int main(int argc, char**argv) {
    if (argc < 3) {
        printf("USAGE: provide value for a and n\n");
        return 0;
    }

    int a = atoi(argv[1]);
    int n = atoi(argv[2]);
    float* x = (float*)malloc(sizeof(float) * n);
    float* y = (float*)malloc(sizeof(float) * n);
    for (int i = 0; i<n; ++i) {
        x[i] = (float)i;
        y[i] = (float)i;
    }

    float * vectors[] = {x, y}; 
    char vectorNames[] = {'x', 'y'}; 
    // show value of vectors before hand
    printNVectors(vectors, vectorNames, 2, n);

    // execute saxpy
    saxpy(a, x, y, n);

    // show value of vectors afterwards
    printNVectors(vectors, vectorNames, 2, n);

    // cleanup
    free(x);
    free(y);
    return 0;
}