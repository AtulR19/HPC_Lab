#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_points>\n", argv[0]);
        return 1;
    }

    long long n = atoll(argv[1]);
    long long inside = 0;

    srand(42);

    clock_t start = clock();

    for (long long i = 0; i < n; i++) {
        double x = 2.0 * rand() / RAND_MAX - 1.0;
        double y = 2.0 * rand() / RAND_MAX - 1.0;

        if (x * x + y * y <= 1.0)
            inside++;
    }

    clock_t end = clock();

    double pi = 4.0 * inside / n;
    double error = fabs(M_PI - pi);
    double time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Points: %lld\n", n);
    printf("Pi: %.10f\n", pi);
    printf("Error: %.10f\n", error);
    printf("Time: %.6f seconds\n", time);

    return 0;
}