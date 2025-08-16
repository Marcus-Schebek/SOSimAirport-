#include "monitoramento.h"
#include <stdio.h>
#include <time.h>

typedef struct {
    int aviao_id;
    time_t inicio;
    int status; // 0 executando, 1 sucesso, 2 falha
} RegistroAviao;

static RegistroAviao registros[1000];
static int total_registros = 0;
static time_t ultimo_evento = 0;
static int deadlocks_detectados = 0;

void registrar_inicio(int aviao_id) {
    registros[total_registros].aviao_id = aviao_id;
    registros[total_registros].inicio = time(NULL);
    registros[total_registros].status = 0;
    total_registros++;
    ultimo_evento = time(NULL);
}

void registrar_sucesso(int aviao_id) {
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].aviao_id == aviao_id) {
            registros[i].status = 1;
            break;
        }
    }
    ultimo_evento = time(NULL);
}

void registrar_falha(int aviao_id) {
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].aviao_id == aviao_id) {
            registros[i].status = 2;
            break;
        }
    }
    ultimo_evento = time(NULL);
}

void verificar_starvation() {
    time_t agora = time(NULL);
    for (int i = 0; i < total_registros; i++) {
        double espera = difftime(agora, registros[i].inicio);
        if (registros[i].status == 0 && espera >= 60) {
            printf("[ALERTA] Avião %d em espera crítica!\n", registros[i].aviao_id);
        }
        if (registros[i].status == 0 && espera >= 90) {
            printf("[FALHA] Avião %d caiu por starvation!\n", registros[i].aviao_id);
            registrar_falha(registros[i].aviao_id);
        }
    }
}

void verificar_deadlock() {
    time_t agora = time(NULL);
    double inatividade = difftime(agora, ultimo_evento);

    // Se não houve progresso nos últimos 10 segundos e ainda existem aviões executando
    if (inatividade >= 10) {
        int ativos = 0;
        for (int i = 0; i < total_registros; i++) {
            if (registros[i].status == 0) {
                ativos++;
            }
        }
        if (ativos > 0) {
            deadlocks_detectados++;
            printf("[DEADLOCK] Nenhum progresso há %.0fs. %d aviões presos!\n", inatividade, ativos);
            ultimo_evento = agora; // evita múltiplos registros seguidos
        }
    }
}

void exibir_metricas() {
    int sucesso = 0, falha = 0;
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].status == 1) sucesso++;
        if (registros[i].status == 2) falha++;
    }
    printf("\n===== MÉTRICAS =====\n");
    printf("Total de aviões: %d\n", total_registros);
    printf("Sucessos: %d\n", sucesso);
    printf("Falhas: %d\n", falha);
    printf("Deadlocks detectados: %d\n", deadlocks_detectados);
}
