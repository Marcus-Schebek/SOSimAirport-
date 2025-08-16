#ifndef RECURSOS_H
#define RECURSOS_H

// Configurações gerais
#define QTD_PISTAS 2
#define QTD_PORTOES 1
#define QTD_TORRE 1

#define TIPO_DOMESTICO 0
#define TIPO_INTERNACIONAL 1

#define STATUS_EXECUTANDO 0
#define STATUS_SUCESSO 1
#define STATUS_FALHA 2
#define STATUS_ALERTA 3

typedef struct {
    int id;
    int tipo;   // 0 doméstico, 1 internacional
    int status; // 0 executando, 1 sucesso, 2 falha
} Aviao;

#endif
