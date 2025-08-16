#include "pistas.h"
#include <semaphore.h>
#include <stdio.h>

static sem_t sem_pistas;

void inicializar_pistas(int qtd) {
    sem_init(&sem_pistas, 0, qtd);
}

void solicitar_pista(int aviao_id) {
    sem_wait(&sem_pistas);
    printf("[AVIÃO %d] Pegou pista.\n", aviao_id);
}

void liberar_pista(int aviao_id) {
    sem_post(&sem_pistas);
    printf("[AVIÃO %d] Liberou pista.\n", aviao_id);
}

void destruir_pistas() {
    sem_destroy(&sem_pistas);
}
