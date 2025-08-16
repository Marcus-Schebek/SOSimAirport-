#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "aviao.h"
#include "pistas.h"
#include "portoes.h"
#include "torre.h"
#include "monitoramento.h"
#include "relatorio.h"

void* rotina_aviao(void* arg) {
    Aviao* av = (Aviao*) arg;
    printf("[AVIÃO %d] Tipo: %s criado.\n", av->id,
           av->tipo == TIPO_INTERNACIONAL ? "Internacional" : "Doméstico");

    registrar_inicio(av->id);

    // ===== Pouso =====
    if (av->tipo == TIPO_INTERNACIONAL) {
        solicitar_pista(av->id);
        solicitar_torre(av->id);
    } else {
        solicitar_torre(av->id);
        solicitar_pista(av->id);
    }
    sleep(1);
    liberar_pista(av->id);
    liberar_torre(av->id);

    // ===== Desembarque =====
    if (av->tipo == TIPO_INTERNACIONAL) {
        solicitar_portao(av->id);
        solicitar_torre(av->id);
    } else {
        solicitar_torre(av->id);
        solicitar_portao(av->id);
    }
    sleep(1);
    liberar_torre(av->id);
    sleep(1);
    liberar_portao(av->id);

    // ===== Decolagem =====
    if (av->tipo == TIPO_INTERNACIONAL) {
        solicitar_portao(av->id);
        solicitar_pista(av->id);
        solicitar_torre(av->id);
    } else {
        solicitar_torre(av->id);
        solicitar_portao(av->id);
        solicitar_pista(av->id);
    }
    sleep(1);
    liberar_portao(av->id);
    liberar_pista(av->id);
    liberar_torre(av->id);

    registrar_sucesso(av->id);
    printf("[AVIÃO %d] Operações concluídas com sucesso.\n", av->id);

    free(av);
    return NULL;
}
