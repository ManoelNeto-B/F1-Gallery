#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necessario para 'system' e 'exit'

// --- Constantes e Globais ---
#define MAX_PILOTOS 100 // Numero maximo de pilotos que podem ser cadastrados
#define USERNAME "admin"
#define PASSWORD "12345"
#define NOME_ARQUIVO_PILOTOS "pilotos.txt" // Nome do arquivo para salvar os dados dos pilotos
#define NOME_ARQUIVO_ESTATISTICAS "estatisticas.txt" // Nome do arquivo para salvar estatisticas

// --- Estruturas de Dados ---

// Struct para o grid (posicoes de largada, se for usar)
typedef struct {
    int pos[22]; // Exemplo: posicoes no grid para 22 corridas ou similar
} Grid;

// Struct para os detalhes do piloto
typedef struct {
    int id; // Identificador unico para o piloto
    char nome[70];
    char origem[20];
    char apelido[20];
    char equipe[20];
    char email[80];
    
} Pilotos;

// Struct para as estatisticas de carreira do piloto
typedef struct {
    int piloto_id; // ID do piloto ao qual estas estatisticas pertencem
    int grandes_premios;
    int vitorias;
    int poles;
    int podios;
    int mvoltas; // Melhores voltas
    int mundiv; // Campeonatos Mundiais
} Cestatisticas;

//Variaveis Globais para Armazenamento em Memoria
Pilotos lista_pilotos[MAX_PILOTOS];
int total_pilotos = 0; // Contador de pilotos cadastrados atualmente

Cestatisticas lista_estatisticas[MAX_PILOTOS]; // Assumindo que cada piloto tera 1 conjunto de estatisticas
int total_estatisticas = 0; // Contador de conjuntos de estatisticas cadastrados

// Prototipos de Funcoes
void realizar_login();
void exibir_menu();
int validar_email(const char *email); // Validar formato e unicidade
void Cadastro_Piloto();
void Cadastro_Estatistica();
void Listar_Pilotos();
void Comparar_Pilotos();
void Modo_Temporada();
void Estatisticas_Temporada();
void Campeoes();
void Estatistica_Pista();
void editar_piloto();
void excluir_piloto();
void carregar_pilotos_do_arquivo(); // Carrega dados dos pilotos do arquivo ao iniciar
void salvar_pilotos_no_arquivo();   // Salva dados dos pilotos no arquivo ao sair/alterar
void carregar_estatisticas_do_arquivo(); // Carrega estatisticas do arquivo
void salvar_estatisticas_no_arquivo();   // Salva estatisticas no arquivo
int buscar_piloto_por_id(int id_busca); // Funcao auxiliar para buscar piloto por ID
int buscar_estatisticas_por_piloto_id(int piloto_id_busca); // Funcao auxiliar para buscar estatisticas por ID do piloto

//Funcao Principal (main)
int main() {
    realizar_login(); // Chama a funcao de login antes de tudo

    carregar_pilotos_do_arquivo(); // Carrega os dados existentes dos pilotos
    carregar_estatisticas_do_arquivo(); // Carrega as estatisticas existentes

    int i = -1;

    while (i != 0) {
        exibir_menu(); // Exibe o menu de opcoes
        printf("O que deseja fazer? ");
        // Limpa o buffer antes de ler o inteiro para evitar problemas
        while(getchar() != '\n');
        scanf("%d", &i);
        
        switch (i) {
            case 1:
                Cadastro_Piloto();
                break;
            case 2:
                Cadastro_Estatistica();
                break;
            case 3:
                Listar_Pilotos();
                break;
            case 4:
                Comparar_Pilotos();
                break;
            case 5:
                Modo_Temporada();
                break;
            case 6:
                Estatisticas_Temporada();
                break;
            case 7:
                Campeoes(); // CHAMADA PARA A NOVA FUNCAO
                break;
            case 8:
                Estatistica_Pista();
                break;
            case 9:
                editar_piloto();
                break;
            case 10:
                excluir_piloto();
                break;
            case 0:
                printf("Saindo do F1 Gallry. Ate a proxima!\n");
                salvar_pilotos_no_arquivo(); // Salva os dados dos pilotos
                salvar_estatisticas_no_arquivo(); // Salva as estatisticas
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

// --- Implementacao das Funcoes ---

void realizar_login() {
    char username_input[50];
    char password_input[50];
    int login_sucesso = 0;

    printf("+------------------------------+\n");
    printf("|     Login - F1 Gallry      |\n");
    printf("+------------------------------+\n");

    while (!login_sucesso) {
        printf("Nome de usuario: ");
        scanf("%s", username_input);

        printf("Senha: ");
        scanf("%s", password_input);

        while (getchar() != '\n'); // Limpa o buffer do teclado

        if (strcmp(username_input, USERNAME) == 0 && strcmp(password_input, PASSWORD) == 0) {
            login_sucesso = 1;
            printf("\nLogin bem-sucedido! Bem-vindo(a) ao F1 Gallry.\n\n");
        } else {
            printf("Nome de usuario ou senha incorretos. Tente novamente.\n\n");
        }
    }
}

void exibir_menu() {
    printf("+------------------------------+\n");
    printf("|    Bem Vindo ao F1 Gallry    |\n");
    printf("+------------------------------+\n");
    printf("| %-30s |\n", "1 - Cadastrar Piloto");
    printf("| %-30s |\n", "2 - Cadastrar Estatisticas do Piloto");
    printf("| %-30s |\n", "3 - Listar Pilotos cadastrados");
    printf("| %-30s |\n", "4 - Comparar Pilotos");
    printf("| %-30s |\n", "5 - Modo temporada");
    printf("| %-30s |\n", "6 - Exibir estatisticas da temporada");
    printf("| %-30s |\n", "7 - Campeoes");
    printf("| %-30s |\n", "8 - Estatisticas no pista");
    printf("| %-30s |\n", "9 - Editar Piloto");
    printf("| %-30s |\n", "10 - Excluir Piloto");
    printf("| %-30s |\n", "0 - Sair");
    printf("+------------------------------+\n");
}

// Retorna 1 se o email for valido e unico, 0 caso contrario
int validar_email(const char *email) {
    // 1. Verificar se contem '@'
    if (strchr(email, '@') == NULL) {
        printf("Erro: O e-mail deve conter o caractere '@'.\n");
        return 0;
    }

    // 2. Verificar o comprimento (menor que 80)
    if (strlen(email) >= 80) {
        printf("Erro: O e-mail deve ter menos de 80 caracteres.\n");
        return 0;
    }

    // 3. Verificar unicidade (apenas se ja houver pilotos cadastrados em memoria)
    for (int i = 0; i < total_pilotos; i++) {
        if (strcmp(lista_pilotos[i].email, email) == 0) {
            printf("Erro: Ja existe um piloto cadastrado com este e-mail.\n");
            return 0;
        }
    }
    
    return 1; // E-mail valido e unico
}

void Cadastro_Piloto() {
    if (total_pilotos >= MAX_PILOTOS) {
        printf("Limite maximo de pilotos atingido. Nao eh possivel cadastrar mais.\n");
        return;
    }

    Pilotos novo_piloto;
    char email_input[80];

    printf("\n--- Cadastro de Piloto ---\n");
    novo_piloto.id = total_pilotos + 1; // Atribui um ID sequencial (comecando de 1)
    printf("ID do Piloto: %d\n", novo_piloto.id);

    printf("Nome: ");
    fgets(novo_piloto.nome, sizeof(novo_piloto.nome), stdin);
    novo_piloto.nome[strcspn(novo_piloto.nome, "\n")] = 0; // Remove o '\n'

    printf("Origem: ");
    fgets(novo_piloto.origem, sizeof(novo_piloto.origem), stdin);
    novo_piloto.origem[strcspn(novo_piloto.origem, "\n")] = 0;

    printf("Apelido: ");
    fgets(novo_piloto.apelido, sizeof(novo_piloto.apelido), stdin);
    novo_piloto.apelido[strcspn(novo_piloto.apelido, "\n")] = 0;

    printf("Equipe Atual: ");
    fgets(novo_piloto.equipe, sizeof(novo_piloto.equipe), stdin);
    novo_piloto.equipe[strcspn(novo_piloto.equipe, "\n")] = 0;

    // Solicita e valida o e-mail
    int email_valido = 0;
    while (!email_valido) {
        printf("E-mail: ");
        fgets(email_input, sizeof(email_input), stdin);
        email_input[strcspn(email_input, "\n")] = 0;
        email_valido = validar_email(email_input);
        if (email_valido) {
            strcpy(novo_piloto.email, email_input); // Copia o email validado
        }
    }

    lista_pilotos[total_pilotos] = novo_piloto;
    total_pilotos++;
    printf("Piloto '%s' (ID: %d) cadastrado com sucesso!\n", novo_piloto.nome, novo_piloto.id);
    salvar_pilotos_no_arquivo(); // Salva apos o cadastro
}

void Cadastro_Estatistica() {
    int id_piloto_busca;
    int indice_piloto = -1;

    printf("\n--- Cadastro de Estatisticas do Piloto ---\n");
    if (total_pilotos == 0) {
        printf("Nenhum piloto cadastrado. Cadastre um piloto primeiro.\n");
        return;
    }

    printf("Digite o ID do piloto para cadastrar/atualizar estatisticas: ");
    scanf("%d", &id_piloto_busca);
    while (getchar() != '\n'); // Limpa o buffer

    // Procura o piloto pelo ID usando a nova funcao auxiliar
    indice_piloto = buscar_piloto_por_id(id_piloto_busca);

    if (indice_piloto == -1) {
        printf("Piloto com ID %d nao encontrado.\n", id_piloto_busca);
        return;
    }

    printf("Cadastrando estatisticas para: %s (ID: %d)\n",
           lista_pilotos[indice_piloto].nome, lista_pilotos[indice_piloto].id);

    // Verifica se ja existem estatisticas para este piloto
    int indice_estatistica = buscar_estatisticas_por_piloto_id(id_piloto_busca);
    
    Cestatisticas novas_estatisticas;
    novas_estatisticas.piloto_id = id_piloto_busca; // Associa ao ID do piloto

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

    if (indice_estatistica != -1) {
        // Atualiza estatisticas existentes
        lista_estatisticas[indice_estatistica] = novas_estatisticas;
        printf("Estatisticas do piloto %s atualizadas com sucesso!\n", lista_pilotos[indice_piloto].nome);
    } else {
        // Adiciona novas estatisticas
        if (total_estatisticas >= MAX_PILOTOS) { // Reutiliza MAX_PILOTOS para o array de estatisticas
            printf("Limite maximo de estatisticas atingido. Nao eh possivel cadastrar mais.\n");
            return;
        }
        lista_estatisticas[total_estatisticas] = novas_estatisticas;
        total_estatisticas++;
        printf("Estatisticas do piloto %s cadastradas com sucesso!\n", lista_pilotos[indice_piloto].nome);
    }
    
    salvar_estatisticas_no_arquivo(); // Salva as estatisticas apos o cadastro/atualizacao
}

void Listar_Pilotos() {
    printf("\n--- Pilotos Cadastrados ---\n");
    if (total_pilotos == 0) {
        printf("Nenhum piloto cadastrado ainda.\n");
        return;
    }

    for (int i = 0; i < total_pilotos; i++) {
        printf("----------------------------------\n");
        printf("Piloto #%d (ID: %d)\n", i + 1, lista_pilotos[i].id);
        printf("Nome: %s\n", lista_pilotos[i].nome);
        printf("Origem: %s\n", lista_pilotos[i].origem);
        printf("Apelido: %s\n", lista_pilotos[i].apelido);
        printf("Equipe: %s\n", lista_pilotos[i].equipe);
        printf("E-mail: %s\n", lista_pilotos[i].email);

        // Busca e exibe as estatisticas associadas, se existirem
        int idx_estatistica = buscar_estatisticas_por_piloto_id(lista_pilotos[i].id);
        if (idx_estatistica != -1) {
            printf("  --- Estatisticas ---\n");
            printf("  GPs: %d\n", lista_estatisticas[idx_estatistica].grandes_premios);
            printf("  Vitorias: %d\n", lista_estatisticas[idx_estatistica].vitorias);
            printf("  Poles: %d\n", lista_estatisticas[idx_estatistica].poles);
            printf("  Podios: %d\n", lista_estatisticas[idx_estatistica].podios);
            printf("  Melhores Voltas: %d\n", lista_estatisticas[idx_estatistica].mvoltas);
            printf("  Titulos Mundiais: %d\n", lista_estatisticas[idx_estatistica].mundiv);
        } else {
            printf("  (Estatisticas nao cadastradas)\n");
        }
    }
    printf("----------------------------------\n");
}

void Comparar_Pilotos() {
    printf("\n--- Comparar Pilotos ---\n");
    if (total_pilotos < 2) {
        printf("São necessários pelo menos 2 pilotos cadastrados para comparar.\n");
        return;
    }
    if (total_estatisticas < 2) {
        printf("São necessárias estatísticas para pelo menos 2 pilotos para comparar.\n");
        return;
    }

    int id_piloto1, id_piloto2;
    int idx_piloto1 = -1, idx_piloto2 = -1;
    int idx_estatistica1 = -1, idx_estatistica2 = -1;

    // Solicita o ID do primeiro piloto
    printf("Digite o ID do primeiro piloto para comparação: ");
    scanf("%d", &id_piloto1);
    while(getchar() != '\n'); // Limpa buffer

    idx_piloto1 = buscar_piloto_por_id(id_piloto1);
    if (idx_piloto1 == -1) {
        printf("Piloto com ID %d não encontrado.\n", id_piloto1);
        return;
    }
    idx_estatistica1 = buscar_estatisticas_por_piloto_id(id_piloto1);
    if (idx_estatistica1 == -1) {
        printf("Estatísticas para o piloto '%s' (ID: %d) não encontradas.\n", lista_pilotos[idx_piloto1].nome, id_piloto1);
        return;
    }

    // Solicita o ID do segundo piloto
    printf("Digite o ID do segundo piloto para comparação: ");
    scanf("%d", &id_piloto2);
    while(getchar() != '\n'); // Limpa buffer

    idx_piloto2 = buscar_piloto_por_id(id_piloto2);
    if (idx_piloto2 == -1) {
        printf("Piloto com ID %d não encontrado.\n", id_piloto2);
        return;
    }
    idx_estatistica2 = buscar_estatisticas_por_piloto_id(id_piloto2);
    if (idx_estatistica2 == -1) {
        printf("Estatísticas para o piloto '%s' (ID: %d) não encontradas.\n", lista_pilotos[idx_piloto2].nome, id_piloto2);
        return;
    }

    // Evitar comparar o mesmo piloto
    if (id_piloto1 == id_piloto2) {
        printf("Por favor, digite IDs de pilotos diferentes para comparação.\n");
        return;
    }

    // Obtem as estatisticas dos pilotos
    Cestatisticas stats1 = lista_estatisticas[idx_estatistica1];
    Cestatisticas stats2 = lista_estatisticas[idx_estatistica2];

    printf("\n--- Comparação de Pilotos ---\n");
    printf("%-30s | %-25s | %-25s\n", "Estatística", lista_pilotos[idx_piloto1].nome, lista_pilotos[idx_piloto2].nome);
    printf("----------------------------------------------------------------------------------\n");

    // Grande Premios
    printf("%-30s | %-25d | %-25d\n", "Grandes Prêmios", stats1.grandes_premios, stats2.grandes_premios);

    // Vitorias
    double perc_vitorias1 = (stats1.grandes_premios > 0) ? ((double)stats1.vitorias / stats1.grandes_premios) * 100.0 : 0.0;
    double perc_vitorias2 = (stats2.grandes_premios > 0) ? ((double)stats2.vitorias / stats2.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Vitórias", stats1.vitorias, perc_vitorias1, stats2.vitorias, perc_vitorias2);

    // Poles
    double perc_poles1 = (stats1.grandes_premios > 0) ? ((double)stats1.poles / stats1.grandes_premios) * 100.0 : 0.0;
    double perc_poles2 = (stats2.grandes_premios > 0) ? ((double)stats2.poles / stats2.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Poles", stats1.poles, perc_poles1, stats2.poles, perc_poles2);

    // Podios
    double perc_podios1 = (stats1.grandes_premios > 0) ? ((double)stats1.podios / stats1.grandes_premios) * 100.0 : 0.0;
    double perc_podios2 = (stats2.grandes_premios > 0) ? ((double)stats2.podios / stats2.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Pódios", stats1.podios, perc_podios1, stats2.podios, perc_podios2);

    // Melhores Voltas
    double perc_mvoltas1 = (stats1.grandes_premios > 0) ? ((double)stats1.mvoltas / stats1.grandes_premios) * 100.0 : 0.0;
    double perc_mvoltas2 = (stats2.grandes_premios > 0) ? ((double)stats2.mvoltas / stats2.grandes_premios) * 100.0 : 0.0;
    printf("%-30s | %-22d (%.2f%%) | %-22d (%.2f%%)\n", "Melhores Voltas", stats1.mvoltas, perc_mvoltas1, stats2.mvoltas, perc_mvoltas2);

    // Mundiais (não é uma porcentagem direta de GPs, exibe o valor absoluto)
    printf("%-30s | %-25d | %-25d\n", "Títulos Mundiais", stats1.mundiv, stats2.mundiv);
    printf("----------------------------------------------------------------------------------\n");
}


void Modo_Temporada() {
    printf("\n--- Modo Temporada ---\n");
    printf("Funcionalidade ainda não implementada.\n");
}

void Estatisticas_Temporada() {
    printf("\n--- Estatísticas da Temporada ---\n");
    printf("Funcionalidade ainda não implementada.\n");
}

void Campeoes() {
    printf("\n--- Pilotos Campeões Mundiais ---\n");
    if (total_pilotos == 0) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    int campeoes_encontrados = 0;
    printf("%-5s | %-30s | %-20s\n", "ID", "Nome do Piloto", "Títulos Mundiais");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < total_pilotos; i++) {
        int idx_estatistica = buscar_estatisticas_por_piloto_id(lista_pilotos[i].id);
        
        // Verifica se existem estatísticas e se o piloto tem 1 ou mais títulos mundiais
        if (idx_estatistica != -1 && lista_estatisticas[idx_estatistica].mundiv > 0) {
            printf("%-5d | %-30s | %-20d\n",
                   lista_pilotos[i].id,
                   lista_pilotos[i].nome,
                   lista_estatisticas[idx_estatistica].mundiv);
            campeoes_encontrados++;
        }
    }
    printf("-----------------------------------------------------------\n");

    if (campeoes_encontrados == 0) {
        printf("Nenhum piloto cadastrado com títulos mundiais.\n");
    }
}


void Estatistica_Pista() {
    printf("\n--- Estatísticas na Pista ---\n");
    printf("Funcionalidade ainda não implementada.\n");
}

void editar_piloto() {
    printf("\n--- Editar Piloto ---\n");
    if (total_pilotos == 0) {
        printf("Nenhum piloto cadastrado para editar.\n");
        return;
    }

    int id_editar;
    printf("Digite o ID do piloto que deseja editar: ");
    scanf("%d", &id_editar);
    while(getchar() != '\n'); // Limpa o buffer

    int indice_piloto_encontrado = buscar_piloto_por_id(id_editar);

    if (indice_piloto_encontrado == -1) {
        printf("Piloto com ID %d não encontrado.\n", id_editar);
        return;
    }

    Pilotos *piloto_para_editar = &lista_pilotos[indice_piloto_encontrado];
    printf("Editando piloto: %s (ID: %d)\n", piloto_para_editar->nome, piloto_para_editar->id);

    int opcao_edicao;
    char novo_valor[80]; // Buffer para novos valores

    do {
        printf("\n--- Opções de Edição ---\n");
        printf("1 - Nome: %s\n", piloto_para_editar->nome);
        printf("2 - Origem: %s\n", piloto_para_editar->origem);
        printf("3 - Apelido: %s\n", piloto_para_editar->apelido);
        printf("4 - Equipe Atual: %s\n", piloto_para_editar->equipe);
        printf("5 - E-mail: %s\n", piloto_para_editar->email);
        printf("0 - Voltar ao menu principal (Salvar alterações)\n");
        printf("Escolha o campo para editar: ");
        scanf("%d", &opcao_edicao);
        while(getchar() != '\n'); // Limpa o buffer

        switch (opcao_edicao) {
            case 1:
                printf("Novo Nome: ");
                fgets(novo_valor, sizeof(piloto_para_editar->nome), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->nome, novo_valor);
                printf("Nome atualizado para: %s\n", piloto_para_editar->nome);
                break;
            case 2:
                printf("Nova Origem: ");
                fgets(novo_valor, sizeof(piloto_para_editar->origem), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->origem, novo_valor);
                printf("Origem atualizada para: %s\n", piloto_para_editar->origem);
                break;
            case 3:
                printf("Novo Apelido: ");
                fgets(novo_valor, sizeof(piloto_para_editar->apelido), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->apelido, novo_valor);
                printf("Apelido atualizado para: %s\n", piloto_para_editar->apelido);
                break;
            case 4:
                printf("Nova Equipe Atual: ");
                fgets(novo_valor, sizeof(piloto_para_editar->equipe), stdin);
                novo_valor[strcspn(novo_valor, "\n")] = 0;
                strcpy(piloto_para_editar->equipe, novo_valor);
                printf("Equipe atualizada para: %s\n", piloto_para_editar->equipe);
                break;
            case 5: {
                int email_valido = 0;
                // Loop para garantir que o novo email seja válido e único (diferente de outros pilotos)
                while (!email_valido) {
                    printf("Novo E-mail: ");
                    fgets(novo_valor, sizeof(piloto_para_editar->email), stdin);
                    novo_valor[strcspn(novo_valor, "\n")] = 0; // Remove o newline

                    email_valido = 1; // Assume que é válido até que se prove o contrário

                    // 1. Verifica formato básico do email
                    if (strchr(novo_valor, '@') == NULL || strlen(novo_valor) >= 80) {
                        printf("Erro: Formato de e-mail inválido (deve conter '@' e ter menos de 80 caracteres).\n");
                        email_valido = 0;
                    }
                    
                    // 2. Verifica unicidade com OUTROS pilotos
                    if (email_valido) { // Só verifica unicidade se o formato for ok
                        for (int i = 0; i < total_pilotos; i++) {
                            // Se o email já existe E não é o email do próprio piloto sendo editado
                            if (lista_pilotos[i].id != piloto_para_editar->id && strcmp(lista_pilotos[i].email, novo_valor) == 0) {
                                printf("Erro: Já existe outro piloto cadastrado com este e-mail.\n");
                                email_valido = 0;
                                break;
                            }
                        }
                    }
                }
                strcpy(piloto_para_editar->email, novo_valor);
                printf("E-mail atualizado para: %s\n", piloto_para_editar->email);
                break;
            }
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao_edicao != 0);

    // Salva todas as alterações no arquivo após sair do menu de edicao
    salvar_pilotos_no_arquivo();
    printf("Alterações salvas com sucesso para o piloto '%s' (ID: %d).\n", piloto_para_editar->nome, piloto_para_editar->id);
}


void excluir_piloto() {
    printf("\n--- Excluir Piloto ---\n");
    if (total_pilotos == 0) {
        printf("Nenhum piloto cadastrado para excluir.\n");
        return;
    }

    int id_excluir;
    printf("Digite o ID do piloto que deseja excluir: ");
    scanf("%d", &id_excluir);
    while(getchar() != '\n'); // Limpa o buffer

    int indice_piloto_encontrado = buscar_piloto_por_id(id_excluir);

    if (indice_piloto_encontrado == -1) {
        printf("Piloto com ID %d não encontrado.\n", id_excluir);
        return;
    }

    printf("Confirmar exclusão do piloto '%s' (ID: %d)? (s/n): ",
           lista_pilotos[indice_piloto_encontrado].nome, id_excluir);
    char confirmacao;
    scanf(" %c", &confirmacao); 
    while(getchar() != '\n'); 

    if (confirmacao == 's' || confirmacao == 'S') {
        //Excluir Estatísticas Associadas
        int indice_estatistica_encontrado = buscar_estatisticas_por_piloto_id(id_excluir);
        if (indice_estatistica_encontrado != -1) {
            // Desloca as estatisticas subsequentes para cobrir a que sera removida
            for (int i = indice_estatistica_encontrado; i < total_estatisticas - 1; i++) {
                lista_estatisticas[i] = lista_estatisticas[i + 1];
            }
            total_estatisticas--;
            printf("Estatísticas do piloto '%s' excluídas.\n", lista_pilotos[indice_piloto_encontrado].nome);
        }

        //Excluir Piloto
        //Desloca os pilotos subsequentes para cobrir o que sera removido
        for (int i = indice_piloto_encontrado; i < total_pilotos - 1; i++) {
            lista_pilotos[i] = lista_pilotos[i + 1];
        }
        total_pilotos--;
        printf("Piloto '%s' (ID: %d) excluído com sucesso!\n", lista_pilotos[indice_piloto_encontrado].nome, id_excluir);
        
        // Salva as alterações em ambos os arquivos
        salvar_pilotos_no_arquivo();
        salvar_estatisticas_no_arquivo();
    } else {
        printf("Exclusão cancelada.\n");
    }
}


//Funcoes Auxiliares de Busca
// Retorna o indice do piloto no array lista_pilotos, ou -1 se nao encontrado
int buscar_piloto_por_id(int id_busca) {
    for (int i = 0; i < total_pilotos; i++) {
        if (lista_pilotos[i].id == id_busca) {
            return i;
        }
    }
    return -1;
}

// Retorna o indice da estatistica no array lista_estatisticas, ou -1 se nao encontrado
int buscar_estatisticas_por_piloto_id(int piloto_id_busca) {
    for (int i = 0; i < total_estatisticas; i++) {
        if (lista_estatisticas[i].piloto_id == piloto_id_busca) {
            return i;
        }
    }
    return -1;
}

// --- Funcoes de Manipulacao de Arquivos para Pilotos ---

void carregar_pilotos_do_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_PILOTOS, "r"); // Abre para leitura
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado. Iniciando com nenhum piloto.\n", NOME_ARQUIVO_PILOTOS);
        return;
    }

    total_pilotos = 0; 
    
    while (total_pilotos < MAX_PILOTOS &&
           fscanf(arquivo, "%d\n%69[^\n]\n%19[^\n]\n%19[^\n]\n%19[^\n]\n%79[^\n]\n",
                  &lista_pilotos[total_pilotos].id,
                  lista_pilotos[total_pilotos].nome,
                  lista_pilotos[total_pilotos].origem,
                  lista_pilotos[total_pilotos].apelido,
                  lista_pilotos[total_pilotos].equipe,
                  lista_pilotos[total_pilotos].email) == 6) {
        total_pilotos++;
    }

    fclose(arquivo);
    printf("Dados de %d piloto(s) carregados do arquivo '%s'.\n", total_pilotos, NOME_ARQUIVO_PILOTOS);
}

void salvar_pilotos_no_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_PILOTOS, "w"); // Abre para escrita (sobrescreve)
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", NOME_ARQUIVO_PILOTOS);
        return;
    }

    for (int i = 0; i < total_pilotos; i++) {
        // Escrevendo o ID e depois os 5 campos de texto
        fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n",
                lista_pilotos[i].id,
                lista_pilotos[i].nome,
                lista_pilotos[i].origem,
                lista_pilotos[i].apelido,
                lista_pilotos[i].equipe,
                lista_pilotos[i].email);
    }

    fclose(arquivo);
    printf("Dados de %d piloto(s) salvos no arquivo '%s'.\n", total_pilotos, NOME_ARQUIVO_PILOTOS);
}

//Funcoes de Manipulacao de Arquivos para Estatisticas

void carregar_estatisticas_do_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_ESTATISTICAS, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado. Iniciando com nenhuma estatística.\n", NOME_ARQUIVO_ESTATISTICAS);
        return;
    }

    total_estatisticas = 0; // Reseta o contador
    while (total_estatisticas < MAX_PILOTOS &&
           fscanf(arquivo, "%d %d %d %d %d %d %d\n", // Le 7 inteiros
                  &lista_estatisticas[total_estatisticas].piloto_id,
                  &lista_estatisticas[total_estatisticas].grandes_premios,
                  &lista_estatisticas[total_estatisticas].vitorias,
                  &lista_estatisticas[total_estatisticas].poles,
                  &lista_estatisticas[total_estatisticas].podios,
                  &lista_estatisticas[total_estatisticas].mvoltas,
                  &lista_estatisticas[total_estatisticas].mundiv) == 7) {
        total_estatisticas++;
    }

    fclose(arquivo);
    printf("Dados de %d estatística(s) carregados do arquivo '%s'.\n", total_estatisticas, NOME_ARQUIVO_ESTATISTICAS);
}

void salvar_estatisticas_no_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO_ESTATISTICAS, "w"); // Abre para escrita (sobrescreve)
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", NOME_ARQUIVO_ESTATISTICAS);
        return;
    }

    for (int i = 0; i < total_estatisticas; i++) {
        fprintf(arquivo, "%d %d %d %d %d %d %d\n",
                lista_estatisticas[i].piloto_id,
                lista_estatisticas[i].grandes_premios,
                lista_estatisticas[i].vitorias,
                lista_estatisticas[i].poles,
                lista_estatisticas[i].podios,
                lista_estatisticas[i].mvoltas,
                lista_estatisticas[i].mundiv);
    }

    fclose(arquivo);
    printf("Dados de %d estatística(s) salvos no arquivo '%s'.\n", total_estatisticas, NOME_ARQUIVO_ESTATISTICAS);
}