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

// Function to create b vector
void create_b_vector(int n, double* b){
    
    for (int i = 0; i < n; i++){
        b[i] = (double)(i + 1) / n;
    }
}

// Fucntion to multiply vector and matrix
void matvec_mult(double* lower, double* diag, double* upper,double* x, double* result, int n) {
    if (n == 1) {
        result[0] = diag[0] * x[0];
        return;
    }
    result[0] = diag[0] * x[0] + upper[0] * x[1];
    for (int i = 1; i < n - 1; i++) {
        result[i] = lower[i - 1] * x[i - 1] + diag[i] * x[i] + upper[i] * x[i + 1];
    }
    result[n - 1] = lower[n - 2] * x[n - 2] + diag[n - 1] * x[n - 1];
}

// Function to calculate dot product
double dot_product(double* v1, double* v2, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += v1[i] * v2[i];
    return sum;
}

// Function to calculate vector norm
double vector_norm(double* v, int n) {
    return sqrt(dot_product(v, v, n));
}

// GMRES algorithm
int gmres(double* lower, double* diag, double* upper, double* b, int n, int m, double* x, double* residuals) {
    double** V = (double**)malloc((m + 1) * sizeof(double*));
    for (int i = 0; i <= m; i++) {
        V[i] = (double*)malloc(n * sizeof(double));
    }

    double** H = (double**)malloc(m * sizeof(double*));

    for (int i = 0; i < m; i++) {
        H[i] = (double*)calloc(m + 1, sizeof(double));     
    }

    double* r = (double*)malloc(n * sizeof(double));
    double* Av = (double*)malloc(n * sizeof(double));

    matvec_mult(lower, diag, upper, x, r, n);
    for (int i = 0; i < n; i++) r[i] = b[i] - r[i];

    double beta = vector_norm(r, n);
    residuals[0] = beta;
    for (int i = 0; i < n; i++) V[0][i] = r[i] / beta;

    int actual_m = m;
    for (int j = 0; j < m; j++) {
        matvec_mult(lower, diag, upper, V[j], Av, n);
        for (int i = 0; i <= j; i++) {
            H[i][j] = dot_product(V[i], Av, n);
            for (int k = 0; k < n; k++) Av[k] -= H[i][j] * V[i][k];
        }
        if (j + 1 >= m) {
            break;
        }
        H[j + 1][j] = vector_norm(Av, n);
        residuals[j + 1] = H[j + 1][j];
        if (H[j + 1][j] < 1e-10) {
            actual_m = j + 1;
            break;
        }
        for (int k = 0; k < n; k++) V[j + 1][k] = Av[k] / H[j + 1][j];
    }

    // Solve for y using backward substitution
    double* y = (double*)malloc(m * sizeof(double));
    for (int i = m - 1; i >= 0; i--) {
        y[i] = residuals[i];
        for (int j = i + 1; j < m; j++) {
            y[i] -= H[i][j] * y[j];
        }
        y[i] /= H[i][i];
    }
    
    // Compute final solution x = V * y
    for (int i = 0; i < n; i++) {
        x[i] = 0.0;
        for (int j = 0; j < m; j++) {
            x[i] += V[j][i] * y[j];
        }
    }
    free(y);
    free(r);
    free(Av);
    return actual_m;
}

int main() {
    int n = 8;
    double m = n / 2;
    double* lower = (double*)calloc(n - 1, sizeof(double));
    double* diag = (double*)calloc(n, sizeof(double));
    double* upper = (double*)calloc(n - 1, sizeof(double));
    double* b = (double*)calloc(n, sizeof(double));
    double* x = (double*)calloc(n, sizeof(double));
    double* residuals = (double*)malloc((m + 1) * sizeof(double));

    create_matrix(n, lower, diag, upper);
    create_b_vector(n, b);

    int steps = gmres(lower, diag, upper, b, n, m, x, residuals);
    printf("Final residual for n = %d: %e\n", n, residuals[steps - 1]);
    printf("Final approximation of x:\n");
    for (int j = 0; j < n; j++){
        printf("%e \n", x[j]);
    }
    printf("\n");

    free(lower);
    free(diag);
    free(upper);
    free(b);
    free(x);
    free(residuals);
    
    return 0;    
}