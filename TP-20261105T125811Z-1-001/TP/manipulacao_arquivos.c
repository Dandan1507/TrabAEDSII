#include <stdlib.h>

#include "manipulacao_arquivos.h"
#include "funcionarios.h"

int tamanho_arquivo(FILE *arq) {
    long bytes;
    long tamanho_serializado = sizeof(int) + TAM_NOME + 15 + 11 + sizeof(double);
    long ultima_posicao;

    if (arq == NULL || fseek(arq, 0, SEEK_END) != 0)
        return -1;
    bytes = ftell(arq);
    if (bytes < 0)
        return -1;
    if (bytes == 0)
        return 0;
    /* Os registros sao espacados por sizeof(TFunc), mas a gravacao omite padding. */
    ultima_posicao = bytes - tamanho_serializado;
    if (ultima_posicao < 0 || ultima_posicao % tamanho_registro() != 0)
        return -1;
    return (int)(ultima_posicao / tamanho_registro()) + 1;
}
