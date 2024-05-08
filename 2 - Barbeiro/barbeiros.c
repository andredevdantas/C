#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int n_cadeiras;
int n_clientes;
int n_barbeiros;

int n_clientes_atend = 0;
int n_atend = 0;
int n_cli_sAtend = 0;
int barbeiros_ocupados = 0;
int time_barbeiro = 3;
int time_cliente = 1;

int n_dormindo = 0;

sem_t semaf_cadeiras;     
sem_t semaf_barbeiro;     
sem_t semaf_cliente;      

void *barbeiro(void *arg) {
	int id = *((int *)arg);
    while (1) {
    	if (barbeiros_ocupados < n_barbeiros) {
    		barbeiros_ocupados++;
	        sem_wait(&semaf_cliente);
			printf("\n\n=======================================");
			printf("\nBarbeiro %d: cortando cabelo do cliente! - %d seg para cortar o cabelo", id+1, time_barbeiro);
			printf("\n=======================================\n");
	        sleep(time_barbeiro);
	        barbeiros_ocupados--;
			sts_barbeiro();
	        sem_post(&semaf_barbeiro); 
		}
    }
    return NULL;
}

void *cliente(void *arg) {
    int id = *((int *)arg);
    if (sem_trywait(&semaf_cadeiras) == 0) {
    	n_clientes_atend++;
        printf("\nCliente %d: sentou na cadeira! [%d/%d] - %d seg ate prox cliente", id +1,n_clientes_atend,n_cadeiras, time_cliente);
        n_atend++;
		srand(time(0));
        time_barbeiro = (rand() % 9) + 2;
        sem_post(&semaf_cliente);
        sem_wait(&semaf_barbeiro);
        n_clientes_atend--;
		printf("\nCliente %d: terminou de cortar o cabelo! [%d/%d]", id +1,n_clientes_atend,n_cadeiras);
        sem_post(&semaf_cadeiras);
        sts_barbeiro();
    } else {
        printf("\nCliente %d: nao ha cadeiras disponiveis, cliente foi embora!  - %d seg ate prox cliente", id, time_cliente);
        n_cli_sAtend++;
    }
    return NULL;
}

int sts_barbeiro(){
	
	if(n_clientes_atend <= 0){
		printf("\n\n=======================================");
    	printf("\nBarbeiro esta DORMINDO!");
    	n_dormindo++;
    	
    	if ((n_atend + n_cli_sAtend) == n_clientes) {
	    	printf("\n%d clientes foram atendidos!", n_atend);
	    	printf("\n%d clientes NAO foram atendidos!", n_cli_sAtend);
	    	printf("\n%d vezes o barbeiro dormiu!", n_dormindo);
	    	printf("\nA barbearia esta FECHADA!\n");
		}
		printf("\n=======================================\n");
	}
	return 0;
	
}

int main() {
	
	printf("A barbearia esta ABERTA!\n");
	
	printf("\nDigite o numero de cadeiras na sala de espera: ");
	scanf("%d", &n_cadeiras);
	
	printf("\nDigite o numero de clientes: ");
	scanf("%d", &n_clientes);
	
	printf("\nDigite o numero de barbeiros: ");
	scanf("%d", &n_barbeiros);
	
    pthread_t tid_barbeiro;
    pthread_t tid_clientes[n_clientes];
    int i, ids[n_clientes], bd[n_barbeiros];

    sem_init(&semaf_cadeiras, 0, n_cadeiras);
    sem_init(&semaf_barbeiro, 0, 0);
    sem_init(&semaf_cliente, 0, 0);
	
	//for (i = 0; i < n_barbeiros; i++) {
	//	pthread_create(&tid_barbeiro, NULL, barbeiro, NULL);
	//}
    for (i = 0; i < n_clientes; i++) {
        ids[i] = i;
        
        pthread_create(&tid_clientes[i], NULL, cliente, &ids[i]);
        if (i < n_barbeiros) {
        	bd[i] = i;
			pthread_create(&tid_barbeiro, NULL, barbeiro, &bd[i]);
		}
		srand(time(0));
        time_cliente = (rand() %5) + 1;
		sleep(time_cliente); 
    }

    pthread_join(tid_barbeiro, NULL);
    for (i = 0; i < n_clientes; i++) {
        pthread_join(tid_clientes[i], NULL);
    }

    sem_destroy(&semaf_cadeiras);
    sem_destroy(&semaf_barbeiro);
    sem_destroy(&semaf_cliente);
    

	return 0;	
	
}

