#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int counter;

pthread_t tid;

// Semáforos para controlar o acesso ao buffer e ao contador
sem_t mutex;      // Semaforo para exclusão mútua
sem_t full;       // Semaforo para controlar itens produzidos
sem_t empty;      // Semaforo para controlar espaços vazios no buffer

void *producer(void *param);
void *consumer(void *param);

int insert_item(int item) {
    sem_wait(&empty);  // Espera por um espaço vazio
    sem_wait(&mutex);  // Inicia exclusão mútua

    if (counter < BUFFER_SIZE) {
        buffer[counter] = item;                                                  
        counter++;

        sem_post(&mutex);  // Libera a exclusão mútua
        sem_post(&full);   // Indica que um item foi produzido
        return 0;
    } else {
        sem_post(&mutex);  // Libera a exclusão mútua
        return 1;
    }
}

int remove_item(int *item) {
    sem_wait(&full);   // Espera por um item produzido
    sem_wait(&mutex);  // Inicia exclusão mútua

    if (counter > 0) {
        *item = buffer[counter - 1];
        counter--;

        sem_post(&mutex);  // Libera a exclusão mútua
        sem_post(&empty);  // Indica que um espaço ficou vazio
        return 0;
    } else {
        sem_post(&mutex);  // Libera a exclusão mútua
        return 1;
    }
}

void initializeData() {
    counter = 0;
    sem_init(&mutex, 0, 1);  // Inicializa o semáforo de exclusão mútua
    sem_init(&full, 0, 0);   // Inicializa o semáforo de itens produzidos
    sem_init(&empty, 0, BUFFER_SIZE);  // Inicializa o semáforo de espaços vazios
}

void* producer(void *param) {
    int item;

    while (1) {
//        sleep(1);

        item = rand() % 100;

        if (!insert_item(item)) {
            printf("producer produced %d\n", item);
        } else {
            fprintf(stderr, "producer report error condition\n");
        }
    }
}

void* consumer(void *param) {
    int item;

    while (1) {
//        sleep(1);

        if (!remove_item(&item)) {                                               
            printf("consumer consumed %d\n", item);
        } else {
            fprintf(stderr, "consumer report error condition\n");
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int mainSleepTime = 10;
    int numProd = 5;
    int numCons = 1;

    initializeData();
    int i;

    for (i = 0; i < numProd; i++) {
        pthread_create(&tid, NULL, producer, NULL);
    }
    for (i = 0; i < numCons; i++) {
        pthread_create(&tid, NULL, consumer, NULL);
    }

    sleep(mainSleepTime);

    printf("Exit the program\n");
    exit(0);
}


