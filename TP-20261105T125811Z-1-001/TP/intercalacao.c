#include "intercalacao.h"
#include "funcionarios.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int vencedor(int esquerda, int direita, TPilha **pilha, int *vetTop) {
    TFunc *func_esquerda;
    TFunc *func_direita;

    if (esquerda < 0)
        return direita;
    if (direita < 0)
        return esquerda;
    func_esquerda = peek_func(pilha[esquerda], 0, &vetTop[esquerda]);
    func_direita = peek_func(pilha[direita], 0, &vetTop[direita]);
    if (func_esquerda->cod < func_direita->cod)
        return esquerda;
    if (func_direita->cod < func_esquerda->cod)
        return direita;
    return esquerda < direita ? esquerda : direita;
}

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].func->cod < menor){
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "",0.0);
                }
                else {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÇões de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p){
    FILE *saida;
    int folhas = 1;
    int *arvore;
    int i;
    long registros_gravados = 0;

    if (pilha == NULL || vetTop == NULL || nome_arquivo_saida == NULL || num_p <= 0) {
        fprintf(stderr, "Parametros invalidos para arvore de vencedores.\n");
        return;
    }
    saida = fopen(nome_arquivo_saida, "wb");
    if (saida == NULL) {
        fprintf(stderr, "Erro ao criar arquivo intercalado.\n");
        return;
    }
    while (folhas < num_p)
        folhas *= 2;
    arvore = (int *) malloc(sizeof(int) * (2 * folhas));
    if (arvore == NULL) {
        fclose(saida);
        fprintf(stderr, "Memoria insuficiente para arvore de vencedores.\n");
        return;
    }

    for (i = 0; i < folhas; i++) {
        if (i < num_p && peek_func(pilha[i], 0, &vetTop[i]) != NULL)
            arvore[folhas + i] = i;
        else
            arvore[folhas + i] = -1;
    }
    for (i = folhas - 1; i > 0; i--)
        arvore[i] = vencedor(arvore[2 * i], arvore[2 * i + 1], pilha, vetTop);

    while (arvore[1] >= 0) {
        int particao = arvore[1];
        TFunc *func = pop(pilha[particao], 0, &vetTop[particao]);
        int no;

        if (func == NULL)
            break;
        if (fseek(saida, registros_gravados * tamanho_registro(), SEEK_SET) != 0) {
            free(func);
            fprintf(stderr, "Erro ao gravar arquivo intercalado.\n");
            break;
        }
        registros_gravados++;
        salva_funcionario(func, saida);
        free(func);
        arvore[folhas + particao] =
            peek_func(pilha[particao], 0, &vetTop[particao]) != NULL ? particao : -1;

        no = (folhas + particao) / 2;
        while (no > 0) {
            arvore[no] = vencedor(arvore[2 * no], arvore[2 * no + 1], pilha, vetTop);
            no /= 2;
        }
    }

    free(arvore);
    fclose(saida);
}
