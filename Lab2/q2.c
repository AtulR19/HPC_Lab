#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KERNEL_SIZE 3

void edge_detection(
    unsigned char *input,
    unsigned char *output,
    int n
) {
    int kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    for (int i = 1; i < n - 1; i++) {
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
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <image_size>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n < 3) {
        printf("Image size must be at least 3.\n");
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

    clock_t start = clock();

    edge_detection(input, output, n);

    clock_t end = clock();

    double elapsed =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Image Size : %d x %d\n", n, n);
    printf("Time       : %.6f seconds\n", elapsed);

    free(input);
    free(output);

    return 0;
}