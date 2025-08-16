#include "portoes.h"
#include <semaphore.h>
#include <stdio.h>

static sem_t sem_portoes;

void inicializar_portoes(int qtd) {
    sem_init(&sem_portoes, 0, qtd);
}

void solicitar_portao(int aviao_id) {
    sem_wait(&sem_portoes);
    printf("[AVIÃO %d] Pegou portão.\n", aviao_id);
}

void liberar_portao(int aviao_id) {
    sem_post(&sem_portoes);
    printf("[AVIÃO %d] Liberou portão.\n", aviao_id);
}

void destruir_portoes() {
    sem_destroy(&sem_portoes);
}
