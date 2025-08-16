#ifndef MONITORAMENTO_H
#define MONITORAMENTO_H

void registrar_inicio(int aviao_id);
void registrar_sucesso(int aviao_id);
void registrar_falha(int aviao_id);
void verificar_starvation();
void exibir_metricas();
void verificar_deadlock();

#endif
