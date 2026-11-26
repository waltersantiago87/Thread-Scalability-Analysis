#define main benchmark_main
#include "../src/matrix.c"
#undef main

#include <math.h>

int main(void) {
    const int sizes[] = {2, 3, 16, 63, 128};
    const int threads[] = {1, 2, 4, 8, 12, 16};

    omp_set_dynamic(0);

    for (size_t s = 0; s < sizeof(sizes)/sizeof(sizes[0]); s++) {
        int n = sizes[s];
        size_t total = (size_t)n * n;

        double *a = malloc(total * sizeof(double));
        double *b = malloc(total * sizeof(double));
        double *ref = calloc(total, sizeof(double));
        double *out = calloc(total, sizeof(double));

        if (!a || !b || !ref || !out) {
            free(a);
            free(b);
            free(ref);
            free(out);
            return EXIT_FAILURE;
        }

        for (size_t i = 0; i < total; i++) {
            a[i] = (double)(i % 17) / 17.0;
            b[i] = (double)(i % 13) / 13.0;
        }

        multiply_ijk(a, b, ref, n, 0);

        for (size_t t = 0; t < sizeof(threads)/sizeof(threads[0]); t++) {
            omp_set_num_threads(threads[t]);

            for (int mode = 0; mode < 3; mode++) {
                memset(out, 0, total * sizeof(double));

                if (mode == 0)
                    multiply_ikj(a, b, out, n, 0);
                else if (mode == 1)
                    multiply_ijk(a, b, out, n, 1);
                else
                    multiply_ikj(a, b, out, n, 1);

                for (size_t i = 0; i < total; i++) {
                    if (!isfinite(out[i]) ||
                        fabs(ref[i] - out[i]) >
                        1e-9 * (1.0 + fabs(ref[i]))) {
                        fprintf(stderr,
                            "FALHA: n=%d threads=%d modo=%d indice=%zu\n",
                            n, threads[t], mode, i);
                        free(a);
                        free(b);
                        free(ref);
                        free(out);
                        return EXIT_FAILURE;
                    }
                }
            }
        }

        printf("PASS: n=%d\n", n);

        free(a);
        free(b);
        free(ref);
        free(out);
    }

    printf("Todas as validacoes passaram.\n");
    return EXIT_SUCCESS;
}
