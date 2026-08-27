#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef struct {
    long long points;
    long long inside;
    unsigned int seed;
} ThreadData;

void *calculate_pi(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    data->inside = 0;

    for (long long i = 0; i < data->points; i++) {

        double x = 2.0 * rand_r(&data->seed) / RAND_MAX - 1.0;
        double y = 2.0 * rand_r(&data->seed) / RAND_MAX - 1.0;

        if (x * x + y * y <= 1.0) {
            data->inside++;
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <number_of_points> <number_of_threads>\n", argv[0]);
        return 1;
    }

    long long total_points = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    if (total_points <= 0 || num_threads <= 0) {
        printf("Points and threads must be greater than 0.\n");
        return 1;
    }

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    /*
     * Divide points among threads.
     */
    long long base_points = total_points / num_threads;
    long long remainder = total_points % num_threads;

    /*
     * Start timer.
     */
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    /*
     * Create threads.
     */
    for (int i = 0; i < num_threads; i++) {

        data[i].points =
            base_points + (i < remainder ? 1 : 0);

        data[i].inside = 0;

        /*
         * Different seed for every thread.
         */
        data[i].seed = 42 + i;

        pthread_create(
            &threads[i],
            NULL,
            calculate_pi,
            &data[i]
        );
    }

    /*
     * Wait for all threads and collect results.
     */
    long long total_inside = 0;

    for (int i = 0; i < num_threads; i++) {

        pthread_join(threads[i], NULL);

        total_inside += data[i].inside;
    }

    /*
     * Stop timer.
     */
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    /*
     * Calculate Pi.
     */
    double pi =
        4.0 * (double)total_inside / (double)total_points;

    /*
     * Calculate error.
     */
    double error = fabs(M_PI - pi);

    /*
     * Display result.
     */
    printf("\n");
    printf("====================================\n");
    printf("Monte Carlo Pi Estimation\n");
    printf("====================================\n");
    printf("Points   : %lld\n", total_points);
    printf("Threads  : %d\n", num_threads);
    printf("Inside   : %lld\n", total_inside);
    printf("Pi       : %.10f\n", pi);
    printf("Error    : %.10f\n", error);
    printf("Time     : %.6f seconds\n", elapsed);
    printf("====================================\n");

    /*
     * Save result to CSV.
     */
    FILE *file = fopen("results.csv", "a");

    if (file == NULL) {
        perror("Error opening results.csv");
        return 1;
    }

    fprintf(
        file,
        "%lld,%d,%.10f,%.10f,%.6f\n",
        total_points,
        num_threads,
        pi,
        error,
        elapsed
    );

    fclose(file);

    return 0;
}