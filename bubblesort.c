#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 18250

int main(void) {
    float *temp = malloc(N * sizeof(float));
    if (!temp) {
        return 1;
    }

    srand((unsigned int)time(NULL));

    // Generate random weather temperatures between -40.0 and 50.0 (Celsius)
    for (int i = 0; i < N; i++) {
        float r = (float)rand() / (float)RAND_MAX;      // 0.0 to 1.0
        temp[i] = -40.0f + r * (50.0f - (-40.0f));
    }

    // Bubble sort
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1 - i; j++) {
            if (temp[j] > temp[j + 1]) {
                float t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    free(temp);
    return 0;
}
