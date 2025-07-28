#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pilotos_estatisticas.h" // Inclui o cabeçalho deste módulo
#include "../include/uteis.h" // Para validar_email
#include "../include/arquivos.h" // Para salvar nos arquivos

// --- Variaveis Globais (Definicoes) ---
NoPiloto *lista_pilotos_head = NULL;
NoEstatistica *lista_estatisticas_head = NULL;
int proximo_piloto_id = 1; // ID inicial para novos pilotos

// --- Implementacao das Funcoes de Manipulacao de Pilotos ---

// Adiciona um piloto no final da lista encadeada
void adicionar_piloto(Piloto novo_piloto) {
    NoPiloto *novo_no = (NoPiloto *)malloc(sizeof(NoPiloto));
    if (novo_no == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o novo piloto.\n");
        return;
    }
    novo_no->dados = novo_piloto;
    novo_no->proximo = NULL;

    if (lista_pilotos_head == NULL) {
        lista_pilotos_head = novo_no;
    } else {
        NoPiloto *atual = lista_pilotos_head;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo_no;
    }
}

void Cadastro_Piloto() {
    Piloto novo_piloto_dados;
    char email_input[MAX_CHAR_EMAIL];

    printf("\n--- Cadastro de Piloto ---\n");
    novo_piloto_dados.id = proximo_piloto_id++; // Atribui ID e incrementa para o proximo
    printf("ID do Piloto: %d\n", novo_piloto_dados.id);

    printf("Nome: ");
    fgets(novo_piloto_dados.nome, sizeof(novo_piloto_dados.nome), stdin);
    novo_piloto_dados.nome[strcspn(novo_piloto_dados.nome, "\n")] = 0; // Remove o '\n'

    printf("Origem: ");
    fgets(novo_piloto_dados.origem, sizeof(novo_piloto_dados.origem), stdin);
    novo_piloto_dados.origem[strcspn(novo_piloto_dados.origem, "\n")] = 0;

    printf("Apelido: ");
    fgets(novo_piloto_dados.apelido, sizeof(novo_piloto_dados.apelido), stdin);
    novo_piloto_dados.apelido[strcspn(novo_piloto_dados.apelido, "\n")] = 0;

    printf("Equipe Atual: ");
    fgets(novo_piloto_dados.equipe, sizeof(novo_piloto_dados.equipe), stdin);
    novo_piloto_dados.equipe[strcspn(novo_piloto_dados.equipe, "\n")] = 0;

    int email_valido_flag = 0;
    while (!email_valido_flag) {
        printf("E-mail: ");
        fgets(email_input, sizeof(email_input), stdin);
        email_input[strcspn(email_input, "\n")] = 0;
        // Valida unicidade e formato. O parametro NULL indica que nao eh uma edicao, entao todos os emails existentes sao considerados
        email_valido_flag = validar_email(email_input, NULL); 
        if (email_valido_flag) {
            strcpy(novo_piloto_dados.email, email_input);
        }
    }

    adicionar_piloto(novo_piloto_dados);
    printf("Piloto '%s' (ID: %d) cadastrado com sucesso!\n", novo_piloto_dados.nome, novo_piloto_dados.id);
    salvar_pilotos_no_arquivo(); // Salva apos o cadastro
}

void listar_pilotos() {
    printf("\n--- Pilotos Cadastrados ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado ainda.\n");
        return;
    }

    NoPiloto *atual_piloto = lista_pilotos_head;
    while (atual_piloto != NULL) {
        printf("----------------------------------\n");
        printf("Piloto (ID: %d)\n", atual_piloto->dados.id);
        printf("Nome: %s\n", atual_piloto->dados.nome);
        printf("Origem: %s\n", atual_piloto->dados.origem);
        printf("Apelido: %s\n", atual_piloto->dados.apelido);
        printf("Equipe: %s\n", atual_piloto->dados.equipe);
        printf("E-mail: %s\n", atual_piloto->dados.email);

        // Busca e exibe as estatisticas associadas, se existirem
        NoEstatistica *estatistica_encontrada = buscar_estatisticas_por_piloto_id_lista(atual_piloto->dados.id);
        if (estatistica_encontrada != NULL) {
            printf("  --- Estatisticas ---\n");
            printf("  GPs: %d\n", estatistica_encontrada->dados.grandes_premios);
            printf("  Vitorias: %d\n", estatistica_encontrada->dados.vitorias);
            printf("  Poles: %d\n", estatistica_encontrada->dados.poles);
            printf("  Podios: %d\n", estatistica_encontrada->dados.podios);
            printf("  Melhores Voltas: %d\n", estatistica_encontrada->dados.mvoltas);
            printf("  Titulos Mundiais: %d\n", estatistica_encontrada->dados.mundiv);
        } else {
            printf("  (Estatisticas nao cadastradas)\n");
        }
        atual_piloto = atual_piloto->proximo;
    }
    printf("----------------------------------\n");
}

// Retorna o ponteiro para o NoPiloto se encontrado, ou NULL
NoPiloto* buscar_piloto_por_id_lista(int id_busca) {
    NoPiloto *atual = lista_pilotos_head;
    while (atual != NULL) {
        if (atual->dados.id == id_busca) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL; // Piloto não encontrado
}

void editar_piloto_menu() {
    printf("\n--- Editar Piloto ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado para editar.\n");
        return;
    }

    int id_editar;
    printf("Digite o ID do piloto que deseja editar: ");
    scanf("%d", &id_editar);
    while(getchar() != '\n'); // Limpa o buffer

    NoPiloto *piloto_para_editar = buscar_piloto_por_id_lista(id_editar);

    if (piloto_para_editar == NULL) {
        printf("Piloto com ID %d nao encontrado.\n", id_editar);
        return;
    }

    printf("Editando piloto: %s (ID: %d)\n", piloto_para_editar->dados.nome, piloto_para_editar->dados.id);

    int opcao_edicao;
    char novo_valor[MAX_CHAR_EMAIL]; // Buffer para novos valores

    do {
        printf("\n--- Opcoes de Edicao ---\n");
        printf("1 - Nome: %s\n", piloto_para_editar->dados.nome);
        printf("2 - Origem: %s\n", piloto_para_editar->dados.origem);
        printf("3 - Apelido: %s\n", piloto_para_editar->dados.apelido);
        printf("4 - Equipe Atual: %s\n", piloto_para_editar->dados.equipe);
        printf("5 - E-mail: %s\n", piloto_para_editar->dados.email);
        printf("0 - Voltar ao menu principal (Salvar alteracoes)\n");
        printf("Escolha o campo para editar: ");
        scanf("%d", &opcao_edicao);
        while(getchar() != '\n');

        switch (opcao_edicao) {
            case 1:
                printf("Novo Nome: ");
                fgets(novo_valor, sizeof(piloto_para_editar->dados.nome), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->dados.nome, novo_valor);
                printf("Nome atualizado para: %s\n", piloto_para_editar->dados.nome);
                break;
            case 2:
                printf("Nova Origem: ");
                fgets(novo_valor, sizeof(piloto_para_editar->dados.origem), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->dados.origem, novo_valor);
                printf("Origem atualizada para: %s\n", piloto_para_editar->dados.origem);
                break;
            case 3:
                printf("Novo Apelido: ");
                fgets(novo_valor, sizeof(piloto_para_editar->dados.apelido), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->dados.apelido, novo_valor);
                printf("Apelido atualizado para: %s\n", piloto_para_editar->dados.apelido);
                break;
            case 4:
                printf("Nova Equipe Atual: ");
                fgets(novo_valor, sizeof(piloto_para_editar->dados.equipe), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->dados.equipe, novo_valor);
                printf("Equipe atualizada para: %s\n", piloto_para_editar->dados.equipe);
                break;
            case 5: {
                int email_valido_flag = 0;
                while (!email_valido_flag) {
                    printf("Novo E-mail: ");
                    fgets(novo_valor, sizeof(piloto_para_editar->dados.email), stdin);
                    novo_valor[strcspn(novo_valor, "\n")] = 0; 
                    email_valido_flag = validar_email(novo_valor, piloto_para_editar->dados.email); // Passa o email atual para permitir igualdade
                }
                strcpy(piloto_para_editar->dados.email, novo_valor);
                printf("E-mail atualizado para: %s\n", piloto_para_editar->dados.email);
                break;
            }
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao_edicao != 0);

    salvar_pilotos_no_arquivo();
    printf("Alteracoes salvas com sucesso para o piloto '%s' (ID: %d).\n", piloto_para_editar->dados.nome, piloto_para_editar->dados.id);
}

void excluir_piloto_menu() {
    printf("\n--- Excluir Piloto ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado para excluir.\n");
        return;
    }

    int id_excluir;
    printf("Digite o ID do piloto que deseja excluir: ");
    scanf("%d", &id_excluir);
    while(getchar() != '\n');

    NoPiloto *atual = lista_pilotos_head;
    NoPiloto *anterior = NULL;

    while (atual != NULL && atual->dados.id != id_excluir) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) { // Piloto não encontrado
        printf("Piloto com ID %d nao encontrado.\n", id_excluir);
        return;
    }

    printf("Confirmar exclusao do piloto '%s' (ID: %d)? (s/n): ",
           atual->dados.nome, atual->dados.id);
    char confirmacao;
    scanf(" %c", &confirmacao);
    while(getchar() != '\n');

    if (confirmacao == 's' || confirmacao == 'S') {
        // Remove da lista de pilotos
        if (anterior == NULL) { // É o primeiro nó
            lista_pilotos_head = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        printf("Piloto '%s' (ID: %d) excluido com sucesso!\n", atual->dados.nome, atual->dados.id);
        free(atual); // Libera a memória do nó do piloto

        // --- Excluir Estatisticas Associadas ---
        NoEstatistica *est_atual = lista_estatisticas_head;
        NoEstatistica *est_anterior = NULL;

        while (est_atual != NULL && est_atual->dados.piloto_id != id_excluir) {
            est_anterior = est_atual;
            est_atual = est_atual->proximo;
        }

        if (est_atual != NULL) { // Estatísticas encontradas
            if (est_anterior == NULL) { // É o primeiro nó de estatísticas
                lista_estatisticas_head = est_atual->proximo;
            } else {
                est_anterior->proximo = est_atual->proximo;
            }
            printf("Estatisticas do piloto excluidas.\n");
            free(est_atual); // Libera a memória do nó de estatística
        }

        salvar_pilotos_no_arquivo();
        salvar_estatisticas_no_arquivo();
    } else {
        printf("Exclusao cancelada.\n");
    }
}

// --- Implementacao das Funcoes de Manipulacao de Estatisticas ---

// Adiciona uma estatística no final da lista encadeada
void adicionar_estatistica(Estatistica nova_estatistica) {
    NoEstatistica *novo_no = (NoEstatistica *)malloc(sizeof(NoEstatistica));
    if (novo_no == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a nova estatistica.\n");
        return;
    }
    novo_no->dados = nova_estatistica;
    novo_no->proximo = NULL;

    if (lista_estatisticas_head == NULL) {
        lista_estatisticas_head = novo_no;
    } else {
        NoEstatistica *atual = lista_estatisticas_head;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo_no;
    }
}

void Cadastro_Estatistica() {
    int id_piloto_busca;
    NoPiloto *piloto_encontrado = NULL;

    printf("\n--- Cadastro de Estatisticas do Piloto ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado. Cadastre um piloto primeiro.\n");
        return;
    }

    printf("Digite o ID do piloto para cadastrar/atualizar estatisticas: ");
    scanf("%d", &id_piloto_busca);
    while (getchar() != '\n');

    piloto_encontrado = buscar_piloto_por_id_lista(id_piloto_busca);

    if (piloto_encontrado == NULL) {
        printf("Piloto com ID %d nao encontrado.\n", id_piloto_busca);
        return;
    }

    printf("Cadastrando estatisticas para: %s (ID: %d)\n",
           piloto_encontrado->dados.nome, piloto_encontrado->dados.id);

    NoEstatistica *estatistica_existente = buscar_estatisticas_por_piloto_id_lista(id_piloto_busca);
    
    Estatistica novas_estatisticas;
    novas_estatisticas.piloto_id = id_piloto_busca;

    printf("Numero de Grandes Premios: ");
    scanf("%d", &novas_estatisticas.grandes_premios);
    while (getchar() != '\n');

    printf("Numero de Vitorias: ");
    scanf("%d", &novas_estatisticas.vitorias);
    while (getchar() != '\n');

    printf("Numero de Poles: ");
    scanf("%d", &novas_estatisticas.poles);
    while (getchar() != '\n');

    printf("Numero de Podios: ");
    scanf("%d", &novas_estatisticas.podios);
    while (getchar() != '\n');

    printf("Numero de Melhores Voltas: ");
    scanf("%d", &novas_estatisticas.mvoltas);
    while (getchar() != '\n');

    printf("Numero de Campeonatos Mundiais: ");
    scanf("%d", &novas_estatisticas.mundiv);
    while (getchar() != '\n');

    if (estatistica_existente != NULL) {
        // Atualiza estatisticas existentes
        estatistica_existente->dados = novas_estatisticas;
        printf("Estatisticas do piloto %s atualizadas com sucesso!\n", piloto_encontrado->dados.nome);
    } else {
        adicionar_estatistica(novas_estatisticas);
        printf("Estatisticas do piloto %s cadastradas com sucesso!\n", piloto_encontrado->dados.nome);
    }
    
    salvar_estatisticas_no_arquivo();
}

// Retorna o ponteiro para o NoEstatistica se encontrado, ou NULL
NoEstatistica* buscar_estatisticas_por_piloto_id_lista(int piloto_id_busca) {
    NoEstatistica *atual = lista_estatisticas_head;
    while (atual != NULL) {
        if (atual->dados.piloto_id == piloto_id_busca) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL; // Estatística não encontrada
}

void comparar_pilotos() {
    printf("\n--- Comparar Pilotos ---\n");
    if (lista_pilotos_head == NULL || lista_pilotos_head->proximo == NULL) {
        printf("Sao necessarios pelo menos 2 pilotos cadastrados para comparar.\n");
        return;
    }

    int id_piloto1, id_piloto2;
    NoPiloto *piloto1 = NULL, *piloto2 = NULL;
    NoEstatistica *stats1 = NULL, *stats2 = NULL;

    printf("Digite o ID do primeiro piloto para comparacao: ");
    scanf("%d", &id_piloto1);
    while(getchar() != '\n'); // Limpa buffer

    piloto1 = buscar_piloto_por_id_lista(id_piloto1);
    if (piloto1 == NULL) {
        printf("Piloto com ID %d nao encontrado.\n", id_piloto1);
        return;
    }
    stats1 = buscar_estatisticas_por_piloto_id_lista(id_piloto1);
    if (stats1 == NULL) {
        printf("Estatisticas para o piloto '%s' (ID: %d) nao encontradas.\n", piloto1->dados.nome, id_piloto1);
        return;
    }

    printf("Digite o ID do segundo piloto para comparacao: ");
    scanf("%d", &id_piloto2);
    while(getchar() != '\n'); // Limpa buffer

    piloto2 = buscar_piloto_por_id_lista(id_piloto2);
    if (piloto2 == NULL) {
        printf("Piloto com ID %d nao encontrado.\n", id_piloto2);
        return;
    }
    stats2 = buscar_estatisticas_por_piloto_id_lista(id_piloto2);
    if (stats2 == NULL) {
        printf("Estatisticas para o piloto '%s' (ID: %d) nao encontradas.\n", piloto2->dados.nome, id_piloto2);
        return;
    }

    if (id_piloto1 == id_piloto2) {
        printf("Por favor, digite IDs de pilotos diferentes para comparacao.\n");
        return;
    }

    printf("\n--- Comparacao de Pilotos ---\n");
    printf("%-30s | %-25s | %-25s\n", "Estatistica", piloto1->dados.nome, piloto2->dados.nome);
    printf("----------------------------------------------------------------------------------\n");

    printf("%-30s | %-25d | %-25d\n", "Grandes Premios", stats1->dados.grandes_premios, stats2->dados.grandes_premios);

    double perc_vitorias1 = (stats1->dados.grandes_premios > 0) ? ((double)stats1->dados.vitorias / stats1->dados.grandes_premios) * 100.0 : 0.0;
    double perc_vitorias2 = (stats2->dados.grandes_premios > 0) ? ((double)stats2->dados.vitorias / stats2->dados.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Vitorias", stats1->dados.vitorias, perc_vitorias1, stats2->dados.vitorias, perc_vitorias2);

    double perc_poles1 = (stats1->dados.grandes_premios > 0) ? ((double)stats1->dados.poles / stats1->dados.grandes_premios) * 100.0 : 0.0;
    double perc_poles2 = (stats2->dados.grandes_premios > 0) ? ((double)stats2->dados.poles / stats2->dados.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Poles", stats1->dados.poles, perc_poles1, stats2->dados.poles, perc_poles2);

    double perc_podios1 = (stats1->dados.grandes_premios > 0) ? ((double)stats1->dados.podios / stats1->dados.grandes_premios) * 100.0 : 0.0;
    double perc_podios2 = (stats2->dados.grandes_premios > 0) ? ((double)stats2->dados.podios / stats2->dados.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Podios", stats1->dados.podios, perc_podios1, stats2->dados.podios, perc_podios2);

    double perc_mvoltas1 = (stats1->dados.grandes_premios > 0) ? ((double)stats1->dados.mvoltas / stats1->dados.grandes_premios) * 100.0 : 0.0;
    double perc_mvoltas2 = (stats2->dados.grandes_premios > 0) ? ((double)stats2->dados.mvoltas / stats2->dados.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Melhores Voltas", stats1->dados.mvoltas, perc_mvoltas1, stats2->dados.mvoltas, perc_mvoltas2);

    printf("%-30s | %-25d | %-25d\n", "Titulos Mundiais", stats1->dados.mundiv, stats2->dados.mundiv);
    printf("----------------------------------------------------------------------------------\n");
}

void exibir_campeoes() {
    printf("\n--- Pilotos Campeoes Mundiais ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    int campeoes_encontrados = 0;
    printf("%-5s | %-30s | %-20s\n", "ID", "Nome do Piloto", "Titulos Mundiais");
    printf("-----------------------------------------------------------\n");

    NoPiloto *atual_piloto = lista_pilotos_head;
    while (atual_piloto != NULL) {
        NoEstatistica *estatistica_encontrada = buscar_estatisticas_por_piloto_id_lista(atual_piloto->dados.id);
        
        if (estatistica_encontrada != NULL && estatistica_encontrada->dados.mundiv > 0) {
            printf("%-5d | %-30s | %-20d\n",
                   atual_piloto->dados.id,
                   atual_piloto->dados.nome,
                   estatistica_encontrada->dados.mundiv);
            campeoes_encontrados++;
        }
        atual_piloto = atual_piloto->proximo;
    }
    printf("-----------------------------------------------------------\n");

    if (campeoes_encontrados == 0) {
        printf("Nenhum piloto cadastrado com titulos mundiais.\n");
    }
}