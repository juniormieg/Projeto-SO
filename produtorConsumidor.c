#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];  // Um buffer compartilhado para armazenar itens
int counter;              // Contador para o n�mero de itens no buffer

pthread_t tid;  // Vari�vel para armazenar o identificador da thread

// Prot�tipos de fun��es
void *producer(void *param);
void *consumer(void *param);

// Fun��o para inserir um item no buffer
int insert_item(int item) {
    if (counter < BUFFER_SIZE) {  // Verifica se o buffer n�o est� cheio
        buffer[counter] = item;   // Insere o item no buffer
        counter++;                // Incrementa o contador
        return 0;                 // Retorna 0 para indicar sucesso
    } else {
        return 1;  // Retorna 1 para indicar que o buffer est� cheio
    }                                                                          
}

// Fun��o para remover um item do buffer
int remove_item(int *item) {
    if (counter > 0) {               // Verifica se o buffer n�o est� vazio
        *item = buffer[counter - 1];  // Remove o item do buffer
        counter--;                    // Decrementa o contador
        return 0;                     // Retorna 0 para indicar sucesso
    } else {
        return 1;  // Retorna 1 para indicar que o buffer est� vazio
    }
}

// Inicializa os dados globais
void initializeData() {
    counter = 0;  // Inicializa o contador como zero
}

// Fun��o executada pelas threads produtoras
void *producer(void *param) {
    int item;

    while (1) {
        sleep(1);           // Espera um segundo (simula a produ��o de um item)
        item = rand() % 100;  // Gera um n�mero aleat�rio entre 0 e 99

        if (!insert_item(item)) {
            printf("producer produced %d\n", item);
        } else {
            fprintf(stderr, "producer report error condition\n");
        }
    }
}

// Fun��o executada pelas threads consumidoras
void *consumer(void *param) {
    int item;

    while (1) {
        sleep(1);  // Espera um segundo (simula o consumo de um item)

        if (!remove_item(&item)) {
            printf("consumer consumed %d\n", item);
        } else {
            fprintf(stderr, "consumer report error condition\n");
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));  // Inicializa o gerador de n�meros aleat�rios

    int mainSleepTime = 10;  // Tempo de execu��o principal em segundos
    int numProd = 5;         // N�mero de threads produtoras
    int numCons = 1;         // N�mero de threads consumidoras

    initializeData();  // Inicializa os dados globais
    int i;

    // Cria threads produtoras
    for (i = 0; i < numProd; i++) {
        pthread_create(&tid, NULL, producer, NULL);
    }

    // Cria threads consumidoras
    for (i = 0; i < numCons; i++) {
        pthread_create(&tid, NULL, consumer, NULL);
    }

    sleep(mainSleepTime);  // Execu��o principal dorme por um tempo

    printf("Exit the program\n");
    exit(0);
}

