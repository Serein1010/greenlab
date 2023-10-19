#include <stdio.h>
#include <math.h>
#include <stdlib.h> 

/* return element i,j of infinite matrix A */
double A(int i, int j) {
    return 1.0 / ((i + j) * (i + j + 1) / 2 + i + 1);
}

/* multiply vector v by matrix A */
void MultiplyAv(int n, double v[], double Av[]) {
    for (int i = 0; i < n; i++) {
        Av[i] = 0;
        for (int j = 0; j < n; j++) {
            Av[i] += A(i, j) * v[j];
        }
    }
}

/* multiply vector v by matrix A transposed */
void MultiplyAtv(int n, double v[], double Atv[]) {
    for (int i = 0; i < n; i++) {
        Atv[i] = 0;
        for (int j = 0; j < n; j++) {
            Atv[i] += A(j, i) * v[j];
        }
    }
}

/* multiply vector v by matrix A and then by matrix A transposed */
void MultiplyAtAv(int n, double v[], double AtAv[]) {
    double u[n];
    MultiplyAv(n, v, u);
    MultiplyAtv(n, u, AtAv);
}

double Approximate(int n) {
    // create unit vector
    double u[n];
    for (int i = 0; i < n; i++) {
        u[i] = 1.0;
    }

    // 20 steps of the power method
    double v[n];
    for (int i = 0; i < n; i++) {
        v[i] = 0.0;
    }

    for (int i = 0; i < 10; i++) {
        MultiplyAtAv(n, u, v);
        MultiplyAtAv(n, v, u);
    }

    // B=AtA         A multiplied by A transposed
    // v.Bv /(v.v)   eigenvalue of v
    double vBv = 0.0, vv = 0.0;
    for (int i = 0; i < n; i++) {
        vBv += u[i] * v[i];
        vv += v[i] * v[i];
    }

    return sqrt(vBv / vv);
}

int main(int argc, char *argv[]) {
    int n = 100;
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    printf("%.9f\n", Approximate(n));
    return 0;
}
