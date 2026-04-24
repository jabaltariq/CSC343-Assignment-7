#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHIL 2

pthread_mutex_t forks[2];

void think(int id) {
    printf("Philosopher %d is thinking\n", id);
    sleep(1);
}

void eat(int id) {
    printf("Philosopher %d is eating\n", id);
    sleep(1);
}

void* philosopher(void* arg) {
    int id = *(int*)arg;

    while (1) {
        think(id);

        if (id == 0) {
            // Philosopher 0 picks left then right
            pthread_mutex_lock(&forks[0]);
            printf("Philosopher %d picked up fork 0\n", id);

            pthread_mutex_lock(&forks[1]);
            printf("Philosopher %d picked up fork 1\n", id);
        } else {
            // Philosopher 1 picks right then left (reverse order)
            pthread_mutex_lock(&forks[1]);
            printf("Philosopher %d picked up fork 1\n", id);

            pthread_mutex_lock(&forks[0]);
            printf("Philosopher %d picked up fork 0\n", id);
        }

        eat(id);

        // Release forks
        pthread_mutex_unlock(&forks[0]);
        pthread_mutex_unlock(&forks[1]);

        printf("Philosopher %d released forks\n", id);
    }
}

int main() {
    pthread_t threads[NUM_PHIL];
    int ids[NUM_PHIL];

    for (int i = 0; i < 2; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_PHIL; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHIL; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
