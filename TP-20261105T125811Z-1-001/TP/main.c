/*
    Prof. Janniele Aparecida Soares Araújo
    Arcabouço para desenvolvimento do trabalho da disciplina de AEDSII 2020-1, utilizando os exemplos dos livros
    de Schildt, H. C Completo e Total. Ed. McGraw-Hill e de Ferraz, I. N. Programação com Arquivos. Editora Manole Ltda
      sobre o problema de funcionários e algoritmos.
*/

#include <stdlib.h>

#include "ordenacao_busca.h"
#include "lista.h"
#include "particoes.h"
#include "intercalacao.h"
#include "arvore_binaria.h"
#include "manipulacao_arquivos.h"

int menu(){
    int op;

    printf("\n\n**************MENU************** \n\nBASE DE DADOS\n");

    printf("\nDigite 1 para utilizar a base de dados funcionario de teste.");
    printf("\nDigite 2 se desejar imprimir a base de dados funcionario.");
    printf("\nDigite 3 se desejar imprimir o indice primario da base de funcionarios.");


    printf("\n\nINTERPOLACAO\n");
    printf("\nDigite 4 para utilizar a estrutura de interpolacao teste.");
    printf("\nDigite 5, 6  para gerar particoes ordenadas utilizando, respectivamente, os metodos: classificacao interna ou selecao natural.");
    printf("\nDigite 7 para criar uma pilha com o conteudo das particoes geradas.");
    printf("\nDigite 8, 9 para gerar intercalar as particoes ordenadas utilizando, respectivamente, os metodos: intercalacao basica, intercalacao arvore de vencedores.");

    printf("\n\nINSERTION SORT E ARVORE BINARIA, ARVORE B\n");
    printf("\nDigite 10 para criar árvores binarias em memoria interna e externa.");
    printf("\nDigite 11 para ordenar a base de dados utilizando o método insertion sort em disco. ");
    printf("\nDigite 12 para criar Arvore B+ de prefixo simples. ");


    printf("\nDigite -1 para sair: ");
    scanf("%d", &op);

    return op;

}

int main(){

    /*criando um arquivo lógico para abir para leitura e escrita o arquivo binário*/
    FILE* arq = fopen("base_funcionarios.dat", "wb+");
    if(arq == NULL) {
        printf("Arquivo base_funcionarios.dat não encontrado.\n");
        return 1;
    }

    /*criando um arquivo lógico para abir para leitura e escrita do arquivo de indice primario ordenado*/
    FILE* arqIndicePrimario = fopen("IndicePrimarioFunc.dat", "wb+");
    if(arqIndicePrimario == NULL) {
        printf("Arquivo IndicePrimarioFunc.dat não encontrado.\n");
        return 1;
    }

    /*criando um arquivo lógico para abir para leitura e escrita do arquivo de indice primario não ordenado*/
    FILE* arqIndicePrimarioNaoOrdenado = fopen("IndicePrimarioFuncNaoOrdenado.dat", "wb+");
    if(arqIndicePrimarioNaoOrdenado == NULL) {
        printf("Arquivo IndicePrimarioFuncNaoOrdenado.dat não encontrado.\n");
        return 1;
    }

    /*criando um arquivo lógico para abir para leitura e escrita da árvore binária externa*/
    FILE* arqArvoreBinariaExterna = fopen("ArvoreBinariaExternaFunc.dat", "wb+");
    if(arqArvoreBinariaExterna == NULL) {
        printf("Arquivo ArvoreBinariaExternaFunc.dat não encontrado.\n");
        return 1;
    }


    int op = menu();
    int nFunc = 0;

    int nParticoes = 0, nElementos = 0, numPilhas = 0;
    Lista  * nomes = NULL;

    TPilha **vetPilhas = NULL;
    int crioubase = 0, crioulistadenomes = 0, criouparticoes = 0, crioupilhas = 0;

    //ponteiro para salvar o início da lista de nomes
    Lista *prox = NULL;
    int *vetTopo = NULL;

    while(op!=-1){
        switch (op){
            case (1):
                //criando a base de dados de funcionários teste e o arquivo de índice primário
                nFunc = 20;
                cria_base_dados_funcionarios_teste(arq, arqIndicePrimario, arqIndicePrimarioNaoOrdenado, nFunc);
                crioubase = 1;
                break;
            case (2):
                //imprime a base de dados dos funcionários
                if(crioubase==1)
                    imprime_base_dados_funcionarios(arq, nFunc);
                else  printf("Base de dados inexistente para ser impressa, crie utilizando a opção 1 do menu!!!\n");
                break;
            case (3):
                //imprime o arquivo de índices primários
                if(crioubase==1){
                    printf("\nArquivo ordenado\n ch:rrn\n");
                    imprime_indice_primario_funcionarios(arqIndicePrimario,nFunc);
                    printf("\nArquivo não ordenado\n ch:rrn\n");
                    imprime_indice_primario_funcionarios(arqIndicePrimarioNaoOrdenado,nFunc);
                }else  printf("Indice primario inexistente para ser impresso, será criado após a definição da base de dados utilizando a opção 1 do menu!!!\n");
                break;
            case (4):
                nParticoes = 10;
                nElementos = 2;
                criouparticoes = 0;
                crioupilhas = 0;
                printf("\n\nA estrutura de interpolação será composta por 10 arquivos de no máximo 2 elementos.\n");
                nomes = cria("p1.dat", cria("p2.dat", cria("p3.dat", cria("p4.dat", cria("p5.dat",  cria("p6.dat",  cria("p7.dat",  cria("p8.dat",  cria("p9.dat",  cria("p10.dat", NULL))))))))));
                crioulistadenomes = 1;
                imprime(nomes);
                break;
            case (5):
                if(crioulistadenomes){
                    //cria as particões que contém a base de dados de funcionários usando o método classificação interna
                    printf("\n\nGerando partições, utilizando o método de classificação interna.\n");
                    nParticoes = classificacao_interna(arq, nomes, nElementos, nFunc);
                    if (nParticoes >= 0) {
                        prox = nomes;
                        criouparticoes = 1;
                        crioupilhas = 0;
                    } else {
                        criouparticoes = 0;
                    }
                }else printf("Lista de nomes de arquivos inexistente para gerar partições, crie a estrutura de interpolação utilizando a opção 4 do menu!!!\n");
                break;
            case (6):
                if(crioulistadenomes){
                    printf("\n\nGerando partições, utilizando o método selecao natural.\n");
                    nParticoes = selecao_natural(arq, nomes, nElementos, nFunc, 1);
                    if (nParticoes >= 0) {
                        prox = nomes;
                        criouparticoes = 1;
                        crioupilhas = 0;
                    } else {
                        criouparticoes = 0;
                    }
                }else printf("Lista de nomes de arquivos inexistente para gerar partições, crie a estrutura de interpolação utilizando a opção  4 do menu!!!\n");
                break;
           case (7):
                if(criouparticoes){
                    int p;
                    //criando pilha com o conteúdo das partições geradas
                    if (vetPilhas != NULL) {
                        for (int p = 0; p < numPilhas; p++) {
                            for (int i = 0; i <= vetTopo[p]; i++)
                                free(vetPilhas[p][i].info);
                            free(vetPilhas[p]);
                        }
                        free(vetPilhas);
                        free(vetTopo);
                    }
                    vetPilhas = (TPilha **) calloc((size_t)nParticoes, sizeof(TPilha *));
                    vetTopo = (int *) calloc((size_t)nParticoes, sizeof(int));
                    if (vetPilhas == NULL || vetTopo == NULL) {
                        free(vetPilhas);
                        free(vetTopo);
                        vetPilhas = NULL;
                        vetTopo = NULL;
                        numPilhas = 0;
                        crioupilhas = 0;
                        printf("Erro ao alocar memoria para as pilhas.\n");
                        break;
                    }
                    for (p = 0; p < nParticoes; p++)
                        vetTopo[p] = -1;
                    for(p = 0 ; p < nParticoes ; p++){
                        //abrindo primeira partição para leitura
                        FILE *part = fopen(nomes->nome, "rb");
                        if (part == NULL || fseek(part, 0, SEEK_END) != 0) {
                            if (part != NULL)
                                fclose(part);
                            printf("Erro ao abrir particao %s.\n", nomes->nome);
                            crioupilhas = 0;
                            break;
                        }
                        int quantidade = tamanho_arquivo(part);
                        rewind(part);
                        if (quantidade <= 0) {
                            fclose(part);
                            printf("Particao vazia ou invalida: %s.\n", nomes->nome);
                            crioupilhas = 0;
                            break;
                        }
                        vetPilhas[p] = (TPilha *) malloc(sizeof(TPilha) * (size_t)quantidade);
                        if (vetPilhas[p] == NULL) {
                            fclose(part);
                            printf("Erro ao alocar memoria para a particao.\n");
                            crioupilhas = 0;
                            break;
                        }
                        printf("Partição %s \n", nomes->nome);
                        nomes = nomes->prox;
                        //imprimindo os funcionários gravados nas partições
                        imprime_base_dados_funcionarios(part, quantidade);
                        //inicializando os topos das pilhas
                        vetTopo[p] = -1;
                        //criando pilha através da leitura da partição
                        cria_pilha_particao(vetPilhas[p], part, quantidade, &vetTopo[p],p);
                        //fechando a partição
                        fclose(part);
                    }
                    if (p == nParticoes)
                        crioupilhas = 1;
                    numPilhas = p;
                    nomes = prox;
                }else printf("Partições não foram criadas, crie as partições utilizando as opções 5 ou 6  do menu!!!\n");
                break;
            case (8):
                if(criouparticoes){
                    printf("\n\nIntercalando as partições, utilizando o método de intercalação basico.\n");
                    intercalacao_basico("arquivo_intercalado.dat", nParticoes, nomes);
                    //imprimindo o arquivo intercalado
                    FILE *fi = fopen("arquivo_intercalado.dat","rb+");
                    if (fi != NULL) {
                        imprime_base_dados_funcionarios(fi,nFunc);
                        fclose(fi);
                    }
                }else printf("Partições não foram criadas, crie as partições utilizando as opções 5 ou 6 do menu!!!\n");
                break;
            case (9):
                if(criouparticoes){
                    if(crioupilhas){
                        intercalacao_arvore_de_vencedores(vetPilhas, vetTopo, "arquivo_intercalado.dat", nParticoes);
                        FILE *fi = fopen("arquivo_intercalado.dat","rb");
                        if (fi != NULL) {
                            imprime_base_dados_funcionarios(fi,nFunc);
                            fclose(fi);
                        }
                    }else {
                        printf("As pilhas das partições geradas não foram criadas, crie as pilhas utilizando a opção 7!!!\n");
                    }
                }else printf("Partições não foram criadas, crie as partições utilizando as opções 5 ou 6  do menu!!!\n");
                break;
            case (10):
                //cria árvore binária em memória utilizando o arquivo de índice primário ordenado
                if(crioubase==1){
                    printf("\nArvore binária em memória pelo arquivo de  índice primário ordenado\n <nopai <noesq><nodireita> >\n");
                    cria_arvore_binaria_indice_primario(arqIndicePrimario,nFunc);
                    printf("\nArvore binária em memória pelo arquivo de índice primário não ordenado\n <nopai <noesq><nodireita> >\n");
                    cria_arvore_binaria_indice_primario(arqIndicePrimarioNaoOrdenado,nFunc);
                    printf("\nArvore binária externa pelo arquivo de base de dados do funcionário\n <nopai <noesq><nodireita> >\n");

                    arqArvoreBinariaExterna = freopen(
                        "ArvoreBinariaExternaFunc.dat",
                            "wb+",
                        arqArvoreBinariaExterna
                    );

                    if (arqArvoreBinariaExterna == NULL) {
                        printf("Erro ao recriar o arquivo da arvore externa.\n");
                        break;
                    }

                    criar_arvore_binaria_externa(arq,arqArvoreBinariaExterna);
                }else  printf("Arquivo de índice primario inexistente para a criação da arvore binaria, crie utilizando a opção 1 do menu!!!\n");
                break;
            case (11):
                 //ordenando a base de dados utilizando ordenação direta no arquivo com o método inseriton sort
                if(crioubase==1){
                    insertion_sort_disco(arq, nFunc);
                    imprime_base_dados_funcionarios(arq, nFunc);
                    //procurando por um funcionário na base de dados usando o método de busca binária
                    int busca_codigo;
                    printf("\n\nInforme o codigo do funcionário procurado (digite -1 caso não queira procurar um funcionário): ");
                    scanf("%d", &busca_codigo);
                    if(busca_codigo!= -1){
                        TFunc* func = busca_binaria(busca_codigo, arq, nFunc);

                        if (func != NULL) {
                            imprime_funcionario(func);
                            free(func);
                        } else {
                            printf("Funcionario com codigo %d nao encontrado.\n",busca_codigo);
                        }

                    }
                }else printf("Base de dados inexistente para ser ordenada, crie utilizando a opção 1 do menu!!!\n");
                break;
            case (12):
                printf("Método a ser implementado na parte II do trabalho.\n");
                if(crioubase==1){
                    ;//criar_arvore_B+();
                }else printf("Base de dados inexistente para ser ordenada, crie utilizando a opção 1 do menu!!!\n");
                break;
            default:
                printf("\nOpção inválida!");
        }
        op = menu();
    }

	while(nomes!=NULL) {
		prox = nomes->prox;
		free(nomes);
		nomes = prox;
	}

	if(vetPilhas!=NULL){
        for(int p = 0; p < numPilhas; p++){
            for (int i = 0; i <= vetTopo[p]; i++)
                free(vetPilhas[p][i].info);
            free(vetPilhas[p]);
        }
        free(vetPilhas);
    }
    free(vetTopo);

    fclose(arqIndicePrimario);
    fclose(arqIndicePrimarioNaoOrdenado);
    fclose(arqArvoreBinariaExterna);
    fclose(arq);

    return 0;
}
