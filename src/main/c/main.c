#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10 // Number of persons

typedef struct {
    sem_t mutex;
    sem_t cond[2];
    int persons_in_bridge;
    int current_direction;
    int waiting_persons[2];
} bridge_t;

bridge_t bridge;

void* person(void* arg) {
    int id = *(int*)arg;
    int direction = id % 2;
    char* direction_names[2] = {"LEFT", "RIGHT"};

    printf("Person %d is going to %s\n", id, direction_names[direction]);

    sem_wait(&bridge.mutex);

    bridge.waiting_persons[direction]++;
    while (bridge.persons_in_bridge > 0 && bridge.current_direction != direction) {
        sem_post(&bridge.mutex);
        sem_wait(&bridge.cond[direction]);
        sem_wait(&bridge.mutex);
    }
    bridge.waiting_persons[direction]--;
    bridge.current_direction = direction;
    bridge.persons_in_bridge++;

    sem_post(&bridge.mutex);

    printf("Person %d is crossing to %s\n", id, direction_names[direction]);
    sleep(1);

    sem_wait(&bridge.mutex);

    bridge.persons_in_bridge--;
    if (bridge.persons_in_bridge == 0) {
        bridge.current_direction = -1;
        for(int i=0; i < 2; i++)
            for(int j=0; j < bridge.waiting_persons[i]; j++)
                sem_post(&bridge.cond[i]);
    }

    sem_post(&bridge.mutex);

    printf("Person %d has crossed to %s\n", id, direction_names[direction]);

    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    sem_init(&bridge.mutex, 0, 1);
    sem_init(&bridge.cond[0], 0, 0);
    sem_init(&bridge.cond[1], 0, 0);
    bridge.persons_in_bridge = 0;
    bridge.current_direction = -1;
    bridge.waiting_persons[0] = bridge.waiting_persons[1] = 0;

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, person, &ids[i]);
        usleep(500000); // Half second
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&bridge.mutex);
    sem_destroy(&bridge.cond[0]);
    sem_destroy(&bridge.cond[1]);

    return 0;
}
