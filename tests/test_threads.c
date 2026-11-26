#include <stdio.h>
#include <omp.h>

int main(void) {
    int configurations[] = {1, 2, 4, 6, 8, 10, 12, 16, 24, 32};

    omp_set_dynamic(0);

    for (int i = 0; i < 10; i++) {
        int requested = configurations[i];

        omp_set_num_threads(requested);

        #pragma omp parallel
        {
            #pragma omp single
            printf("Solicitadas: %2d | Efetivas: %2d\n",
                   requested, omp_get_num_threads());
        }
    }

    return 0;
}
