#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5

pthread_mutex_t chopsticks[N];
sem_t waiter;

void* philosopher(void* arg)
{
    int id = *(int*)arg;

    while (1) {

        /* Thinking */
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);

        /* Hungry */
        printf("Philosopher %d is hungry\n", id);

        /*
         * Ask the waiter for permission.
         * At most N-1 philosophers can try to
         * acquire chopsticks at the same time.
         */
        sem_wait(&waiter);

        printf("Philosopher %d got permission from waiter\n", id);

        /*
         * Pick up left chopstick
         */
        pthread_mutex_lock(&chopsticks[id]);

        printf(
            "Philosopher %d picked up left chopstick\n",
            id
        );

        /*
         * Pick up right chopstick
         */
        pthread_mutex_lock(
            &chopsticks[(id + 1) % N]
        );

        printf(
            "Philosopher %d picked up right chopstick\n",
            id
        );

        /* Eating */
        printf("Philosopher %d is eating...\n", id);
        sleep(1);

        /*
         * Put down chopsticks
         */
        pthread_mutex_unlock(&chopsticks[id]);

        pthread_mutex_unlock(
            &chopsticks[(id + 1) % N]
        );

        printf(
            "Philosopher %d finished eating\n",
            id
        );

        /*
         * Release waiter
         */
        sem_post(&waiter);
    }

    return NULL;
}

int main()
{
    pthread_t philosophers[N];
    int ids[N];

    /*
     * Initialize chopstick mutexes
     */
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(
            &chopsticks[i],
            NULL
        );
    }

    /*
     * Allow only N-1 philosophers
     * to compete for chopsticks.
     */
    sem_init(&waiter, 0, N - 1);

    /*
     * Create philosopher threads
     */
    for (int i = 0; i < N; i++) {

        ids[i] = i;

        pthread_create(
            &philosophers[i],
            NULL,
            philosopher,
            &ids[i]
        );
    }

    /*
     * Wait for philosopher threads
     */
    for (int i = 0; i < N; i++) {
        pthread_join(
            philosophers[i],
            NULL
        );
    }

    /*
     * Cleanup
     */
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(
            &chopsticks[i]
        );
    }

    sem_destroy(&waiter);

    return 0;
}