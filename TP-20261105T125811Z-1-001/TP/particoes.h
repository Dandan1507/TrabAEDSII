#ifndef PARTICOES_H
#define PARTICOES_H

#include <stdio.h>

#include "lista.h"

/* Gera particoes por classificacao interna. Retorna a quantidade gerada ou -1 em caso de erro. */
int classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc);

/* Gera particoes por selecao natural. M e a capacidade da heap ativa, n e a capacidade
do reservatorio (1 <= n <= M). Retorna a quantidade gerada ou -1 em caso de erro. */
int selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n);


#endif
