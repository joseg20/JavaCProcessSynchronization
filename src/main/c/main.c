#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10 // Número de personas

typedef struct 
{
    sem_t mutex; // Mutex para la sección crítica
    sem_t cond[2]; //cola para cada dirección
    int persons_in_bridge; //sección crítica
    int current_direction; //sección crítica
    int waiting_persons[2]; //sección crítica
}bridge_t;

bridge_t bridge;

void * person (void * arg){
    int id = *(int *) arg; // Identificador de la persona 
    int direction = id % 2; // Dirección de la persona
    char * direction_names[2] = {"LEFT", "RIGHT"};

    printf("Person %d wants to go to %s\n", id, direction_names[direction]);
    printf("Persons in bridge: %d. Waiting persons: Direction LEFT -> %d, Direction RIGHT -> %d\n", 
    bridge.persons_in_bridge, bridge.waiting_persons[0], bridge.waiting_persons[1]);
    sem_wait(&bridge.mutex); // Entrar a la sección crítica
    bridge.waiting_persons[direction]++; // Aumentar el número de personas esperando en la dirección de la persona

    while (bridge.persons_in_bridge > 0 && bridge.current_direction != direction) 
    // Mientras haya personas en el puente y la dirección de la persona sea diferente a la dirección actual
    // caso inicial y cuando terminan de pasar en una direccion las personas en el puente es 0
    // cuando hay personas en el puente tienes quue tener la misma direccion para poder pasar
    {
        sem_post(&bridge.mutex); // Salir de la sección crítica
        sem_wait(&bridge.cond[direction]); // Esperar en la cola de la dirección de la persona
        sem_wait(&bridge.mutex); // Entrar a la sección crítica
    }

    bridge.waiting_persons[direction]--; // Disminuir el número de personas esperando en la dirección de la persona
    bridge.persons_in_bridge++; // Aumentar el número de personas en el puente
    bridge.current_direction = direction; // Actualizar la dirección actual
    sem_post(&bridge.mutex); // Salir de la sección crítica

    printf("Person %d is crossing to %s\n", id, direction_names[direction]);
    printf("Persons in bridge: %d. Waiting persons: Direction LEFT -> %d, Direction RIGHT -> %d\n", 
    bridge.persons_in_bridge, bridge.waiting_persons[0], bridge.waiting_persons[1]);
    sleep(1); // Cruzar el puente
    printf("Person %d has crossed the bridge to %s\n", id, direction_names[direction]);
    printf("Persons in bridge: %d. Waiting persons: Direction LEFT -> %d, Direction RIGHT -> %d\n", 
    bridge.persons_in_bridge, bridge.waiting_persons[0], bridge.waiting_persons[1]);
    sem_wait(&bridge.mutex); // Entrar a la sección crítica
    bridge.persons_in_bridge--; // Disminuir el número de personas en el puente
    if (bridge.persons_in_bridge == 0 ){
        bridge.current_direction = -1; // Actualizar la dirección actual cuando no hay personas
        for(int i=0; i < 2; i++)
            for (int j=0; j < bridge.waiting_persons[i]; j++)
                sem_post(&bridge.cond[i]); // Liberar a las personas en la cola de la dirección i
    }

    sem_post(&bridge.mutex); // Salir de la sección crítica
    printf("Person %d has left the bridge to %s\n", id, direction_names[direction]);
    return NULL;
}

int main(){
    pthread_t threads[N]; // Arreglo de hilos
    int ids[N]; // Arreglo de identificadores de las personas

    sem_init(&bridge.mutex, 0, 1); // Inicializar el mutex
    sem_init(&bridge.cond[0], 0, 0); // Inicializar la cola de la dirección 0
    sem_init(&bridge.cond[1], 0, 0); // Inicializar la cola de la dirección 1
    bridge.persons_in_bridge = 0; // Inicializar el número de personas en el puente
    bridge.current_direction = -1; // Inicializar la dirección actual, se pone -1 para indicar que no hay dirección actual
    bridge.waiting_persons[0] = 0; // Inicializar el número de personas esperando en la dirección 0
    bridge.waiting_persons[1] = 0; // Inicializar el número de personas esperando en la dirección 1

    for (int i=0; i<N; i++){
        ids[i] = i; // Inicializar el identificador de la persona
        pthread_create(&threads[i], NULL, person, &ids[i]); // Crear el hilo de la persona
        usleep(500000); // Esperar 0.5 segundos
    }

    for (int i=0; i<N; i++)
        pthread_join(threads[i], NULL); // Esperar a que terminen los hilos

    sem_destroy(&bridge.mutex); // Destruir el mutex
    sem_destroy(&bridge.cond[0]); // Destruir la cola de la dirección 0
    sem_destroy(&bridge.cond[1]); // Destruir la cola de la dirección 1

    return 0;
}
