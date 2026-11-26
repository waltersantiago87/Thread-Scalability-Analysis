#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>


static inline void row_ijk(const double *a,
                           const double *b,
                           double *c, int n, int i) {
    for (int j = 0; j < n; j++) {
        double sum = 0.0;

        for (int k = 0; k < n; k++) {
            sum += a[(size_t)i * n + k]
                 * b[(size_t)k * n + j];
        }

        c[(size_t)i * n + j] = sum;
    }
}

static inline void row_ikj(const double *a,
                           const double *b,
                           double *c, int n, int i) {
    for (int k = 0; k < n; k++) {
        double value = a[(size_t)i * n + k];

        for (int j = 0; j < n; j++) {
            c[(size_t)i * n + j] +=
                value * b[(size_t)k * n + j];
        }
    }
}

void multiply_ijk(const double *a, const double *b,
                  double *c, int n, int parallel) {
    if (parallel) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++) {
            row_ijk(a, b, c, n, i);
        }
    } else {
        for (int i = 0; i < n; i++) {
            row_ijk(a, b, c, n, i);
        }
    }
}

void multiply_ikj(const double *a, const double *b,
                  double *c, int n, int parallel) {
    if (parallel) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++) {
            row_ikj(a, b, c, n, i);
        }
    } else {
        for (int i = 0; i < n; i++) {
            row_ikj(a, b, c, n, i);
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr,
            "Uso: %s <modo> <dimensao> <threads>\n",
            argv[0]);
        return EXIT_FAILURE;
    }

    const char *mode = argv[1];
    int n = atoi(argv[2]);
    int threads = atoi(argv[3]);

    if (n <= 0 || threads <= 0 ||
        (size_t)n > SIZE_MAX / sizeof(double) / (size_t)n) {
        fprintf(stderr, "Parametros invalidos.\n");
        return EXIT_FAILURE;
    }

    int parallel = 0;
    int optimized = 0;

    if (strcmp(mode, "seq-ijk") == 0) {
        parallel = 0;
        optimized = 0;
    } else if (strcmp(mode, "seq-ikj") == 0) {
        parallel = 0;
        optimized = 1;
    } else if (strcmp(mode, "par-ijk") == 0) {
        parallel = 1;
        optimized = 0;
    } else if (strcmp(mode, "par-ikj") == 0) {
        parallel = 1;
        optimized = 1;
    } else {
        fprintf(stderr, "Modo invalido.\n");
        return EXIT_FAILURE;
    }

    if (!parallel) {
        threads = 1;
    }

    omp_set_dynamic(0);
    omp_set_num_threads(threads);

    size_t elements = (size_t)n * n;

    double *a = malloc(elements * sizeof(double));
    double *b = malloc(elements * sizeof(double));
    double *c = calloc(elements, sizeof(double));

    if (!a || !b || !c) {
        fprintf(stderr, "Erro de alocacao.\n");

        free(a);
        free(b);
        free(c);

        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < elements; i++) {
        a[i] = (double)(i % 100) / 100.0;
        b[i] = (double)(i % 73) / 73.0;
        c[i] = 0.0;
    }

    double start = omp_get_wtime();

    if (optimized) {
        multiply_ikj(a, b, c, n, parallel);
    } else {
        multiply_ijk(a, b, c, n, parallel);
    }

    double end = omp_get_wtime();

    double checksum = 0.0;

    for (size_t i = 0; i < elements; i++) {
        checksum += c[i];
    }

    printf("%s,%d,%d,%.9f,%.10f\n",
           mode, n, threads, end - start, checksum);

    free(a);
    free(b);
    free(c);

    return EXIT_SUCCESS;
}
