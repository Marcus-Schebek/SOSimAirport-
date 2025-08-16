#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "aviao.h"
#include "pistas.h"
#include "portoes.h"
#include "torre.h"
#include "relatorio.h"
#include "recursos.h"

#define TEMPO_SIMULACAO 20 // segundos
#define MAX_THREADS 1000

int main() {
    pthread_t threads[MAX_THREADS];
    int total_threads = 0;

    inicializar_pistas(QTD_PISTAS);
    inicializar_portoes(QTD_PORTOES);
    inicializar_torre(QTD_TORRE);

    srand(time(NULL));
    time_t inicio = time(NULL);

    while (difftime(time(NULL), inicio) < TEMPO_SIMULACAO) {
        Aviao* av = malloc(sizeof(Aviao));
        av->id = total_threads + 1;
        av->tipo = rand() % 2;
        av->status = STATUS_EXECUTANDO;

        pthread_create(&threads[total_threads], NULL, rotina_aviao, av);
        total_threads++;

        verificar_starvation();  // checa se alguém está em alerta ou falhou
        verificar_deadlock();    // checa se sistema inteiro travou

        usleep((rand() % 2000 + 1000) * 1000); // 1 a 3 segundos
    }   


    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gerar_relatorio_final();

    destruir_pistas();
    destruir_portoes();
    destruir_torre();

    return 0;
}
