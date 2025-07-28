#include <stdio.h>
#include <stdlib.h> // Para exit()
#include "../include/pilotos_estatisticas.h"
#include "../include/arquivos.h"
#include "../include/uteis.h"
#include "../include/buscas_ordenacao.h" // Para as novas funcoes de busca e ordenacao

// --- Funcoes para o Menu (Nao implementadas ainda, apenas placeholders) ---
void Modo_Temporada() {
    printf("\n--- Modo Temporada ---\n");
    printf("Funcionalidade ainda nao implementada.\n");
}

void Estatisticas_Temporada() {
    printf("\n--- Estatisticas da Temporada ---\n");
    printf("Funcionalidade ainda nao implementada.\n");
}

void Estatistica_Pista() {
    printf("\n--- Estatisticas na Pista ---\n");
    printf("Funcionalidade ainda nao implementada.\n");
}

// --- Funcao Principal (main) ---
int main() {
    realizar_login();

    // Carrega os dados existentes dos arquivos para as listas encadeadas
    carregar_pilotos_do_arquivo();
    carregar_estatisticas_do_arquivo();

    int i = -1;

    while (i != 0) {
        exibir_menu();
        printf("O que deseja fazer? ");
        // Limpa o buffer antes de ler o inteiro
        scanf("%d", &i);
        while(getchar() != '\n'); 
        
        switch (i) {
            case 1:
                Cadastro_Piloto(); // Chamada para a funcao no pilotos_estatisticas.c
                break;
            case 2:
                Cadastro_Estatistica(); // Chamada para a funcao no pilotos_estatisticas.c
                break;
            case 3:
                listar_pilotos(); // Chamada para a funcao
                break;
            case 4:
                comparar_pilotos();
                break;
            case 5:
                Modo_Temporada();
                break;
            case 6:
                Estatisticas_Temporada();
                break;
            case 7:
                exibir_campeoes(); // Chamada para a funcao
                break;
            case 8:
                Estatistica_Pista();
                break;
            case 9:
                editar_piloto_menu(); // Chamada para a funcao
                break;
            case 10:
                excluir_piloto_menu(); // Chamada para a funcao
                break;
            case 11:
                ordenar_pilotos_por_nome(); 
                break;
            case 12:
                buscar_piloto_por_nome_binaria_menu(); 
                break;
            case 0:
                printf("Saindo do F1 Gallry. Ate a proxima!\n");
                // Salva os dados das listas encadeadas nos arquivos
                salvar_pilotos_no_arquivo();
                salvar_estatisticas_no_arquivo();

                // Libera a memória das listas antes de sair
                NoPiloto *current_piloto = lista_pilotos_head;
                while (current_piloto != NULL) {
                    NoPiloto *next = current_piloto->proximo;
                    free(current_piloto);
                    current_piloto = next;
                }
                NoEstatistica *current_estatistica = lista_estatisticas_head;
                while (current_estatistica != NULL) {
                    NoEstatistica *next = current_estatistica->proximo;
                    free(current_estatistica);
                    current_estatistica = next;
                }
                break;
            default:
                printf(">> Valor invalido <<\n");
                break;
        }

        // Pausa para o usuario ver o resultado antes do menu ser redesenhado
        if (i != 0) {
            printf("\nPressione ENTER para continuar...");
            while(getchar() != '\n'); // Consome qualquer ENTER que sobrou
            getchar(); // Espera pelo ENTER do usuario
        }
    }

    return 0;
}