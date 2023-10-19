#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define PRECISION 4000

void computePiToNthDigit(int n, char* result) {
    mpf_set_default_prec(PRECISION);

    mpf_t a, b, t, p, pi, tmp1, tmp2;
    mpf_inits(a, b, t, p, pi, tmp1, tmp2, NULL);

    mpf_set_d(a, 1.0);
    mpf_sqrt_ui(b, 2);
    mpf_ui_div(b, 1, b);
    mpf_set_d(t, 0.25);
    mpf_set_ui(p, 1);

    for (int i = 0; i < n; i++) {
        mpf_add(tmp1, a, b);
        mpf_div_ui(tmp1, tmp1, 2);
        mpf_mul(tmp2, a, b);
        mpf_sqrt(b, tmp2);
        mpf_sub(tmp2, a, tmp1);
        mpf_mul(tmp2, tmp2, tmp2);
        mpf_mul(tmp2, p, tmp2);
        mpf_sub(t, t, tmp2);
        mpf_swap(a, tmp1);
        mpf_mul_ui(p, p, 2);
    }

    mpf_add(tmp1, a, b);
    mpf_mul(tmp1, tmp1, tmp1);
    mpf_div(pi, tmp1, t);
    mpf_div_ui(pi, pi, 4);

    gmp_sprintf(result, "%.Ff", pi);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number of digits>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > 1000) {
        printf("Please provide a positive integer for the number of digits (max 1000).\n");
        return 1;
    }

    char result[PRECISION / 3 + 2];
    computePiToNthDigit(n, result);

    // Adjusted printing logic for the desired format
    int digitsPrinted = 0;
    for (int i = 0; result[i] && digitsPrinted < n; i++) {
        if (result[i] != '.') {  // Skip the decimal point
            putchar(result[i]);
            digitsPrinted++;
        }

        if (digitsPrinted % 10 == 0 && digitsPrinted > 0) {
            printf("\t:%d\n", digitsPrinted);
        }
    }

    return 0;
}
