#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10 // Number of persons

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int persons_in_bridge;
    int current_direction;
    int waiting_persons[2];
} bridge_t;

bridge_t bridge = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, -1, {0, 0}};

void* person(void* arg) {
    int id = *(int*)arg;
    int direction = id % 2;
    char* direction_names[2] = {"LEFT", "RIGHT"};

    printf("Person %d is going to %s\n", id, direction_names[direction]);

    pthread_mutex_lock(&bridge.mutex);

    bridge.waiting_persons[direction]++;
    while (bridge.persons_in_bridge > 0 && bridge.current_direction != direction) {
        pthread_cond_wait(&bridge.cond, &bridge.mutex);
    }
    bridge.waiting_persons[direction]--;
    bridge.current_direction = direction;
    bridge.persons_in_bridge++;

    pthread_mutex_unlock(&bridge.mutex);

    printf("Person %d is crossing to %s\n", id, direction_names[direction]);
    sleep(1);

    pthread_mutex_lock(&bridge.mutex);

    bridge.persons_in_bridge--;
    if (bridge.persons_in_bridge == 0) {
        bridge.current_direction = -1;
        pthread_cond_broadcast(&bridge.cond);
    }

    pthread_mutex_unlock(&bridge.mutex);

    printf("Person %d has crossed to %s\n", id, direction_names[direction]);

    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, person, &ids[i]);
        usleep(500000); // Half second
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
