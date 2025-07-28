#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/arquivos.h"
#include "../include/pilotos_estatisticas.h" // Para acessar as listas globais e structs

// --- Funcoes de Manipulacao de Arquivos para Pilotos ---

void carregar_pilotos_do_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_PILOTOS, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' nao encontrado. Iniciando com nenhum piloto.\n", NOME_ARQUIVO_PILOTOS);
        return;
    }

    // Limpa a lista existente antes de carregar do arquivo para evitar duplicatas
    NoPiloto *atual_piloto = lista_pilotos_head;
    while (atual_piloto != NULL) {
        NoPiloto *proximo = atual_piloto->proximo;
        free(atual_piloto);
        atual_piloto = proximo;
    }
    lista_pilotos_head = NULL;
    proximo_piloto_id = 1; // Reseta o ID para garantir sequencialidade a partir do maior ID lido

    Piloto temp_piloto;
    int max_id_encontrado = 0;

    // Lendo o ID (inteiro) e depois os 5 campos de texto
    while (fscanf(arquivo, "%d\n%69[^\n]\n%19[^\n]\n%19[^\n]\n%19[^\n]\n%79[^\n]\n",
                  &temp_piloto.id,
                  temp_piloto.nome,
                  temp_piloto.origem,
                  temp_piloto.apelido,
                  temp_piloto.equipe,
                  temp_piloto.email) == 6) {
        adicionar_piloto(temp_piloto); // Adiciona o piloto lido na lista encadeada
        if (temp_piloto.id >= max_id_encontrado) {
            max_id_encontrado = temp_piloto.id;
        }
    }
    proximo_piloto_id = max_id_encontrado + 1; // Define o proximo ID disponivel

    fclose(arquivo);
    printf("Dados de piloto(s) carregados do arquivo '%s'.\n", NOME_ARQUIVO_PILOTOS);
}

void salvar_pilotos_no_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_PILOTOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", NOME_ARQUIVO_PILOTOS);
        return;
    }

    NoPiloto *atual = lista_pilotos_head;
    while (atual != NULL) {
        fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n",
                atual->dados.id,
                atual->dados.nome,
                atual->dados.origem,
                atual->dados.apelido,
                atual->dados.equipe,
                atual->dados.email);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Dados de piloto(s) salvos no arquivo '%s'.\n", NOME_ARQUIVO_PILOTOS);
}

// --- Funcoes de Manipulacao de Arquivos para Estatisticas ---

void carregar_estatisticas_do_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_ESTATISTICAS, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' nao encontrado. Iniciando com nenhuma estatistica.\n", NOME_ARQUIVO_ESTATISTICAS);
        return;
    }

    // Limpa a lista existente antes de carregar do arquivo para evitar duplicatas
    NoEstatistica *atual_estatistica = lista_estatisticas_head;
    while (atual_estatistica != NULL) {
        NoEstatistica *proximo = atual_estatistica->proximo;
        free(atual_estatistica);
        atual_estatistica = proximo;
    }
    lista_estatisticas_head = NULL;

    Estatistica temp_estatistica;
    while (fscanf(arquivo, "%d %d %d %d %d %d %d\n",
                  &temp_estatistica.piloto_id,
                  &temp_estatistica.grandes_premios,
                  &temp_estatistica.vitorias,
                  &temp_estatistica.poles,
                  &temp_estatistica.podios,
                  &temp_estatistica.mvoltas,
                  &temp_estatistica.mundiv) == 7) {
        adicionar_estatistica(temp_estatistica); // Adiciona a estatística lida na lista encadeada
    }

    fclose(arquivo);
    printf("Dados de estatistica(s) carregados do arquivo '%s'.\n", NOME_ARQUIVO_ESTATISTICAS);
}

void salvar_estatisticas_no_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_ESTATISTICAS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", NOME_ARQUIVO_ESTATISTICAS);
        return;
    }

    NoEstatistica *atual = lista_estatisticas_head;
    while (atual != NULL) {
        fprintf(arquivo, "%d %d %d %d %d %d %d\n",
                atual->dados.piloto_id,
                atual->dados.grandes_premios,
                atual->dados.vitorias,
                atual->dados.poles,
                atual->dados.podios,
                atual->dados.mvoltas,
                atual->dados.mundiv);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Dados de estatistica(s) salvos no arquivo '%s'.\n", NOME_ARQUIVO_ESTATISTICAS);
}