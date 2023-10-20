#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];  // Um buffer compartilhado para armazenar itens
int counter;              // Contador para o número de itens no buffer

pthread_t tid;  // Variável para armazenar o identificador da thread

// Protótipos de funções
void *producer(void *param);
void *consumer(void *param);

// Função para inserir um item no buffer
int insert_item(int item) {
    if (counter < BUFFER_SIZE) {  // Verifica se o buffer não está cheio
        buffer[counter] = item;   // Insere o item no buffer
        counter++;                // Incrementa o contador
        return 0;                 // Retorna 0 para indicar sucesso
    } else {
        return 1;  // Retorna 1 para indicar que o buffer está cheio
    }                                                                          
}

// Função para remover um item do buffer
int remove_item(int *item) {
    if (counter > 0) {               // Verifica se o buffer não está vazio
        *item = buffer[counter - 1];  // Remove o item do buffer
        counter--;                    // Decrementa o contador
        return 0;                     // Retorna 0 para indicar sucesso
    } else {
        return 1;  // Retorna 1 para indicar que o buffer está vazio
    }
}

// Inicializa os dados globais
void initializeData() {
    counter = 0;  // Inicializa o contador como zero
}

// Função executada pelas threads produtoras
void *producer(void *param) {
    int item;

    while (1) {
        sleep(1);           // Espera um segundo (simula a produção de um item)
        item = rand() % 100;  // Gera um número aleatório entre 0 e 99

        if (!insert_item(item)) {
            printf("producer produced %d\n", item);
        } else {
            fprintf(stderr, "producer report error condition\n");
        }
    }
}

// Função executada pelas threads consumidoras
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
    srand((unsigned int)time(NULL));  // Inicializa o gerador de números aleatórios

    int mainSleepTime = 10;  // Tempo de execução principal em segundos
    int numProd = 5;         // Número de threads produtoras
    int numCons = 1;         // Número de threads consumidoras

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

    sleep(mainSleepTime);  // Execução principal dorme por um tempo

    printf("Exit the program\n");
    exit(0);
}

