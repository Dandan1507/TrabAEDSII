
#include "funcionarios.h"
#include <stdlib.h>
#include <stdio.h>

#include "particoes.h"

static void heap_push(TFunc **heap, int *tamanho, TFunc *func) {
    int i = (*tamanho)++;
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap[pai]->cod <= func->cod)
            break;
        heap[i] = heap[pai];
        i = pai;
    }
    heap[i] = func;
}

static TFunc *heap_pop(TFunc **heap, int *tamanho) {
    TFunc *menor = heap[0];
    TFunc *ultimo = heap[--(*tamanho)];
    int i = 0;

    while (2 * i + 1 < *tamanho) {
        int filho = 2 * i + 1;
        if (filho + 1 < *tamanho && heap[filho + 1]->cod < heap[filho]->cod)
            filho++;
        if (heap[filho]->cod >= ultimo->cod)
            break;
        heap[i] = heap[filho];
        i = filho;
    }
    if (*tamanho > 0)
        heap[i] = ultimo;
    return menor;
}

static TFunc *ler_registro(FILE *arq, int indice) {
    if (fseek(arq, (long)indice * tamanho_registro(), SEEK_SET) != 0)
        return NULL;
    return le_funcionario(arq);
}

int classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    int registro = 0;
    int particoes = 0;

    if (arq == NULL || M <= 0 || nFunc < 0)
        return -1;
    rewind(arq);

    while (registro < nFunc) {
        TFunc **vetor = (TFunc **) malloc(sizeof(TFunc *) * M);
        int quantidade = 0;
        int i;
        FILE *saida;

        if (vetor == NULL)
            return -1;
        while (quantidade < M && registro < nFunc) {
            TFunc *func = ler_registro(arq, registro);
            if (func == NULL) {
                for (i = 0; i < quantidade; i++)
                    free(vetor[i]);
                free(vetor);
                fprintf(stderr, "Erro ao ler o arquivo de entrada.\n");
                return -1;
            }
            vetor[quantidade++] = func;
            registro++;
        }

        for (i = 1; i < quantidade; i++) {
            TFunc *func = vetor[i];
            int j = i - 1;
            while (j >= 0 && vetor[j]->cod > func->cod) {
                vetor[j + 1] = vetor[j];
                j--;
            }
            vetor[j + 1] = func;
        }

        if (nome_arquivos_saida == NULL) {
            for (i = 0; i < quantidade; i++)
                free(vetor[i]);
            free(vetor);
            fprintf(stderr, "Nomes de particao insuficientes.\n");
            return -1;
        }
        saida = fopen(nome_arquivos_saida->nome, "wb");
        if (saida == NULL) {
            for (i = 0; i < quantidade; i++)
                free(vetor[i]);
            free(vetor);
            fprintf(stderr, "Erro ao criar arquivo de particao.\n");
            return -1;
        }
        for (i = 0; i < quantidade; i++) {
            if (fseek(saida, (long)i * tamanho_registro(), SEEK_SET) != 0) {
                int j;
                for (j = i; j < quantidade; j++)
                    free(vetor[j]);
                fclose(saida);
                free(vetor);
                fprintf(stderr, "Erro ao gravar arquivo de particao.\n");
                return -1;
            }
            salva_funcionario(vetor[i], saida);
            free(vetor[i]);
        }
        fclose(saida);
        free(vetor);
        nome_arquivos_saida = nome_arquivos_saida->prox;
        particoes++;
    }
    return particoes;
}

int selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n) {
    TFunc **reservatorio = NULL;
    int quantidade_reservatorio = 0;
    int registros_lidos = 0;
    int entrada_terminou = 0;
    int particoes = 0;

    if (arq == NULL || M <= 0 || n <= 0 || n > M || nFunc < 0) {
        fprintf(stderr, "Parametros invalidos para selecao natural.\n");
        return -1;
    }
    if (nFunc == 0)
        return 0;
    rewind(arq);

    while (1) {
        TFunc **heap;
        TFunc **proximo_reservatorio;
        int tamanho_heap = 0;
        int quantidade_proxima = 0;
        int registros_gravados = 0;
        int parar_leitura = 0;
        FILE *saida;

        if (nome_arquivos_saida == NULL) {
            fprintf(stderr, "Nomes de particao insuficientes.\n");
            goto erro;
        }
        heap = (TFunc **) malloc(sizeof(TFunc *) * M);
        proximo_reservatorio = (TFunc **) malloc(sizeof(TFunc *) * n);
        if (heap == NULL || proximo_reservatorio == NULL) {
            free(heap);
            free(proximo_reservatorio);
            fprintf(stderr, "Memoria insuficiente para selecao natural.\n");
            goto erro;
        }

        while (quantidade_reservatorio > 0)
            heap_push(heap, &tamanho_heap, reservatorio[--quantidade_reservatorio]);
        free(reservatorio);
        reservatorio = NULL;

        while (tamanho_heap < M && registros_lidos < nFunc) {
            TFunc *func = ler_registro(arq, registros_lidos);
            if (func == NULL) {
                int i;
                for (i = 0; i < tamanho_heap; i++)
                    free(heap[i]);
                free(heap);
                free(proximo_reservatorio);
                fprintf(stderr, "Erro ao ler o arquivo de entrada.\n");
                goto erro;
            }
            registros_lidos++;
            heap_push(heap, &tamanho_heap, func);
        }
        if (registros_lidos == nFunc)
            entrada_terminou = 1;
        if (tamanho_heap == 0) {
            free(heap);
            free(proximo_reservatorio);
            break;
        }

        saida = fopen(nome_arquivos_saida->nome, "wb");
        if (saida == NULL) {
            int i;
            for (i = 0; i < tamanho_heap; i++)
                free(heap[i]);
            free(heap);
            free(proximo_reservatorio);
            fprintf(stderr, "Erro ao criar arquivo de particao.\n");
            goto erro;
        }

        while (tamanho_heap > 0) {
            TFunc *menor = heap_pop(heap, &tamanho_heap);
            int ultima_chave = menor->cod;
            if (fseek(saida, (long)registros_gravados * tamanho_registro(), SEEK_SET) != 0) {
                int i;
                free(menor);
                for (i = 0; i < tamanho_heap; i++)
                    free(heap[i]);
                for (i = 0; i < quantidade_proxima; i++)
                    free(proximo_reservatorio[i]);
                fclose(saida);
                free(heap);
                free(proximo_reservatorio);
                fprintf(stderr, "Erro ao gravar arquivo de particao.\n");
                goto erro;
            }
            registros_gravados++;
            salva_funcionario(menor, saida);
            free(menor);

            if (registros_lidos < nFunc && !parar_leitura) {
                TFunc *proximo = ler_registro(arq, registros_lidos);
                if (proximo == NULL) {
                    int i;
                    for (i = 0; i < tamanho_heap; i++)
                        free(heap[i]);
                    for (i = 0; i < quantidade_proxima; i++)
                        free(proximo_reservatorio[i]);
                    fclose(saida);
                    free(heap);
                    free(proximo_reservatorio);
                    fprintf(stderr, "Erro ao ler o arquivo de entrada.\n");
                    goto erro;
                }
                registros_lidos++;
                if (registros_lidos == nFunc)
                    entrada_terminou = 1;
                if (proximo->cod >= ultima_chave) {
                    heap_push(heap, &tamanho_heap, proximo);
                } else {
                    proximo_reservatorio[quantidade_proxima++] = proximo;
                    if (quantidade_proxima == n)
                        parar_leitura = 1;
                }
            }
        }

        fclose(saida);
        free(heap);
        reservatorio = proximo_reservatorio;
        quantidade_reservatorio = quantidade_proxima;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        particoes++;

        if (entrada_terminou && quantidade_reservatorio == 0)
            break;
    }
    free(reservatorio);
    return particoes;

erro:
    if (reservatorio != NULL) {
        int i;
        for (i = 0; i < quantidade_reservatorio; i++)
            free(reservatorio[i]);
        free(reservatorio);
    }
    return -1;
}
