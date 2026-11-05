#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "lista.h"
#include "pilha.h"

/*estrutura para armazenar o vetor de partições*/
typedef struct vetor{
    TFunc *func;
    int aux_p;
    FILE *f;
}TVet;

/*Algoritmo de intercalacao de particoes básico, recebe
o nome do arquivo de saída, o número de partições a serem intercaladas
e uma lista com os nomes das partições de entrada a serem lidas.*/
void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes);

/* Intercala as pilhas ordenadas por codigo usando uma arvore de vencedores. */
void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p);

#endif
