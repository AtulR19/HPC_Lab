#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    unsigned char *input;
    unsigned char *output;

    int n;
    int start_row;
    int end_row;
} ThreadData;


void *edge_detection(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    unsigned char *input = data->input;
    unsigned char *output = data->output;

    int n = data->n;

    int kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    for (int i = data->start_row;
         i < data->end_row;
         i++) {

        // Boundary rows are not processed
        if (i == 0 || i == n - 1)
            continue;

        for (int j = 1; j < n - 1; j++) {

            int sum = 0;

            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {

                    sum +=
                        input[(i + ki) * n + (j + kj)]
                        * kernel[ki + 1][kj + 1];
                }
            }

            if (sum < 0)
                sum = 0;

            if (sum > 255)
                sum = 255;

            output[i * n + j] = sum;
        }
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf(
            "Usage: %s <image_size> <number_of_threads>\n",
            argv[0]
        );
        return 1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (n < 3 || num_threads < 1) {
        printf("Invalid arguments.\n");
        return 1;
    }

    size_t size = (size_t)n * n;

    unsigned char *input = malloc(size);
    unsigned char *output = malloc(size);

    if (input == NULL || output == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Generate test image
    srand(42);

    for (size_t i = 0; i < size; i++) {
        input[i] = rand() % 256;
    }

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    // Divide rows between threads
    int base_rows = n / num_threads;
    int remainder = n % num_threads;

    int current_row = 0;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {

        int rows =
            base_rows + (i < remainder ? 1 : 0);

        data[i].input = input;
        data[i].output = output;
        data[i].n = n;

        data[i].start_row = current_row;
        data[i].end_row = current_row + rows;

        current_row += rows;

        pthread_create(
            &threads[i],
            NULL,
            edge_detection,
            &data[i]
        );
    }

    // Wait for all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\n");
printf("====================================\n");
printf("Parallel Edge Detection\n");
printf("====================================\n");
printf("Image Size : %d x %d\n", n, n);
printf("Threads    : %d\n", num_threads);
printf("Time       : %.6f seconds\n", elapsed);
printf("====================================\n");


FILE *file = fopen("results.csv", "a");

if (file == NULL) {
    perror("Error opening results.csv");
    free(input);
    free(output);
    return 1;
}

fprintf(
    file,
    "%d,%d,%.6f\n",
    n,
    num_threads,
    elapsed
);

fclose(file);

    return 0;
}