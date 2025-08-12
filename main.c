#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define TEMPO_TOTAL_SIMULACAO 30 // segundos
#define MAX_THREADS 1000 // apenas para evitar excesso

// Semáforos dos recursos
sem_t sem_pistas;
sem_t sem_portoes;
sem_t sem_torre;

// Estrutura para identificar um avião
typedef struct {
    int id;
    int tipo; // 0 = doméstico, 1 = internacional
} Aviao;

// Função da thread do avião
void* rotina_aviao(void* arg) {
    Aviao *av = (Aviao*) arg;

    printf("[AVIÃO %d] Tipo: %s criado.\n",
        av->id, av->tipo ? "Internacional" : "Doméstico");

    // ======== ETAPA 1: POUSO ========
    if (av->tipo == 1) { // Internacional: Pista → Torre
        sem_wait(&sem_pistas);
        printf("[AVIÃO %d] Pegou pista para pouso.\n", av->id);

        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou pouso.\n", av->id);

    } else { // Doméstico: Torre → Pista
        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou pouso.\n", av->id);

        sem_wait(&sem_pistas);
        printf("[AVIÃO %d] Pegou pista para pouso.\n", av->id);
    }

    sleep(1); // Simulando tempo de pouso

    sem_post(&sem_pistas);
    sem_post(&sem_torre);
    printf("[AVIÃO %d] Pouso concluído.\n", av->id);

    // ======== ETAPA 2: DESEMBARQUE ========
    if (av->tipo == 1) { // Internacional: Portão → Torre
        sem_wait(&sem_portoes);
        printf("[AVIÃO %d] Pegou portão para desembarque.\n", av->id);

        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou desembarque.\n", av->id);

    } else { // Doméstico: Torre → Portão
        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou desembarque.\n", av->id);

        sem_wait(&sem_portoes);
        printf("[AVIÃO %d] Pegou portão para desembarque.\n", av->id);
    }

    sleep(1); // Simulando desembarque
    sem_post(&sem_torre);
    sleep(1); // Portão fica ocupado mais um tempo
    sem_post(&sem_portoes);
    printf("[AVIÃO %d] Desembarque concluído.\n", av->id);

    // ======== ETAPA 3: DECOLAGEM ========
    if (av->tipo == 1) { // Internacional: Portão → Pista → Torre
        sem_wait(&sem_portoes);
        printf("[AVIÃO %d] Pegou portão para embarque.\n", av->id);

        sem_wait(&sem_pistas);
        printf("[AVIÃO %d] Pegou pista para decolagem.\n", av->id);

        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou decolagem.\n", av->id);

    } else { // Doméstico: Torre → Portão → Pista
        sem_wait(&sem_torre);
        printf("[AVIÃO %d] Torre liberou embarque.\n", av->id);

        sem_wait(&sem_portoes);
        printf("[AVIÃO %d] Pegou portão para embarque.\n", av->id);

        sem_wait(&sem_pistas);
        printf("[AVIÃO %d] Pegou pista para decolagem.\n", av->id);
    }

    sleep(1); // Simulando decolagem
    sem_post(&sem_portoes);
    sem_post(&sem_pistas);
    sem_post(&sem_torre);

    printf("[AVIÃO %d] Decolagem concluída.\n", av->id);
    printf("[AVIÃO %d] Operações finalizadas com sucesso.\n", av->id);

    free(av);
    pthread_exit(NULL);
}


int main() {
    pthread_t threads[MAX_THREADS];
    int total_threads = 0;

    // Inicializar recursos
    sem_init(&sem_pistas, 0, 3);  // 3 pistas
    sem_init(&sem_portoes, 0, 5); // 5 portões
    sem_init(&sem_torre, 0, 2);   // torre atende 2 simultâneos

    srand(time(NULL));
    time_t inicio = time(NULL);

    while (difftime(time(NULL), inicio) < TEMPO_TOTAL_SIMULACAO) {
        // Criar novo avião
        Aviao* av = malloc(sizeof(Aviao));
        av->id = total_threads + 1;
        av->tipo = rand() % 2; // 0 doméstico, 1 internacional

        pthread_create(&threads[total_threads], NULL, rotina_aviao, av);
        total_threads++;

        // Intervalo randômico entre criações
        usleep((rand() % 2000 + 1000) * 1000); // entre 1 e 3 segundos
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Encerrar semáforos
    sem_destroy(&sem_pistas);
    sem_destroy(&sem_portoes);
    sem_destroy(&sem_torre);

    printf("\nSimulação encerrada. Total de aviões: %d\n", total_threads);
    return 0;
}
