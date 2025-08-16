#include "torre.h"
#include <semaphore.h>
#include <stdio.h>

static sem_t sem_torre;

void inicializar_torre(int qtd) {
    sem_init(&sem_torre, 0, qtd);
}

void solicitar_torre(int aviao_id) {
    sem_wait(&sem_torre);
    printf("[AVIÃO %d] Pegou torre.\n", aviao_id);
}

void liberar_torre(int aviao_id) {
    sem_post(&sem_torre);
    printf("[AVIÃO %d] Liberou torre.\n", aviao_id);
}

void destruir_torre() {
    sem_destroy(&sem_torre);
}
