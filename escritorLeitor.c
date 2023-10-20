#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <semaphore.h>
#include <unistd.h>

#define LEITORES 10 //qtd leitor
#define ESCRITORES 5 //qtd escritor

//controlar a região crítica
pthread_mutex_t db; 
pthread_mutex_t mutex; 

//guardar a qtd de leitor lendo
int leitor_lendo; 

//prototipos das funções
void ler_base_de_dados(void);
void usar_banco_de_dados(void);
void pensando_nos_dados(void);
void escrever_no_banco_de_dados(void);

//principais ações dos leitores
void leitor(){
	
	//loop infinito
	while(1){
		
		/*bloqueia, quando um leitor entra na seçao critica*/
		pthread_mutex_lock(&mutex);
		
		//indica que um novo leitor esta lendo                                           
		leitor_lendo=leitor_lendo+1;
		
		//se for o primeiro leitor
		/*bloqueia db para impedir que os escritores acessem 
		o banco de dados*/
		if(leitor_lendo==1) pthread_mutex_lock(&db);
		
		/*permite q outros leitores entrem e também permitir que incremetem
		leitor_lendo ou verificar se sao os primeiros leitores*/
		pthread_mutex_unlock(&mutex); 
		
		ler_base_de_dados();
		pthread_mutex_lock(&mutex); //atualizar leitor_lendo
		leitor_lendo=leitor_lendo-1;
		
		if(leitor_lendo==0) pthread_mutex_unlock(&db); //permitir a entrada de escritores
		pthread_mutex_unlock(&mutex);
		
		usar_banco_de_dados();
	}
}

void escritor(){
	while(1){
		pensando_nos_dados();
		
		//um escritor por vez
		pthread_mutex_lock(&db);
		escrever_no_banco_de_dados();
		//desbloqueia, permitir a entrada de outros
		pthread_mutex_unlock(&db);
	}
}

void ler_base_de_dados(){
	
	int tempo_de_leitura;
	tempo_de_leitura = rand() % 5;
	
	printf("usuario LENDO banco de dados. Total de %d leitores LENDO agora.\n", leitor_lendo);
	sleep(tempo_de_leitura);
}

void usar_banco_de_dados(){

	int tempo_de_uso;
	tempo_de_uso = rand() % 15;
	
	printf("usuario utilizando conhecimento adquirido no banco de dados.\n");
	sleep(tempo_de_uso);
}

void pensando_nos_dados(){
	
	int tempo_para_pensar; //armazenar a qtd de tempo antes de realizar a operações
	tempo_para_pensar = rand() % 10;
	printf("Escritor PENSANDO no que ira escrever.\n");
	sleep(tempo_para_pensar);
}

void escrever_no_banco_de_dados(){
	
	int tempo_de_escrita;
	tempo_de_escrita = rand() % 10;
	printf("Escritor ESCREVENDO no banco de dados.\n");
	sleep(tempo_de_escrita);	
}
int main(){
 	
 	pthread_t threadsdosescritores[ESCRITORES], threadsdosleitores[LEITORES];
 	int i;
 	
 	//inicializar os mutex
 	pthread_mutex_init(&db, NULL);
 	pthread_mutex_init(&mutex, NULL);
 	
 	for(i=0; i<ESCRITORES; i++){
 		//cria threads e armazena suas identificações nos arrays
 		pthread_create(&threadsdosescritores[i], NULL, (void*)escritor, NULL);	
	 }
	for(i=0; i<LEITORES; i++){
 		pthread_create(&threadsdosleitores[i], NULL, (void*)leitor, NULL);	
	 }
	 
	 //aguardar que as threads terminem e nao encerre os programas
	for(i=0; i<ESCRITORES; i++){
 		pthread_join(threadsdosescritores[i], NULL);	
	 }
	for(i=0; i<LEITORES; i++){
 		pthread_join(threadsdosleitores[i], NULL);	
	 }
	 return(0);
 }

