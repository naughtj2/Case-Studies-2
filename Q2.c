#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to create matrix of varying dimensions
void create_matrix(int n, double* lower, double* diag, double* upper) {
    for (int i = 0; i < n; i++){
        diag[i] = -4.0;
        if (i < n - 1) upper[i] = 1;
        if (i > 0) lower[i - 1] = 1;
    }
}

void print_array(const char* name, double* array, int size) {
    printf("%s: [", name);
    for (int i = 0; i < size; i++) {
        printf("%.1f", array[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int n = 8;
    double* lower = (double*)calloc(n - 1, sizeof(double));
    double* diag = (double*)calloc(n, sizeof(double));
    double* upper = (double*)calloc(n - 1, sizeof(double));

    create_matrix(n, lower, diag, upper);

    print_array("Lower", lower, n - 1);
    print_array("Diag", diag, n);
    print_array("Upper", upper, n - 1);

    free(lower);
    free(diag);
    free(upper);
    
}