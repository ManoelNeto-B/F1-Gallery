#include <stdio.h>
#include <string.h>
#include "../include/uteis.h"
#include "../include/pilotos_estatisticas.h" // Para acessar lista_pilotos_head

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
    printf("| %-30s |\n", "5 - Modo temporada (N/I)");
    printf("| %-30s |\n", "6 - Exibir estatisticas da temporada (N/I)");
    printf("| %-30s |\n", "7 - Campeoes");
    printf("| %-30s |\n", "8 - Estatisticas no pista (N/I)");
    printf("| %-30s |\n", "9 - Editar Piloto");
    printf("| %-30s |\n", "10 - Excluir Piloto");
    printf("| %-30s |\n", "11 - Ordenar Pilotos por Nome"); // Nova opcao para demonstrar ordenacao
    printf("| %-30s |\n", "12 - Buscar Piloto por Nome (Binaria)"); // Nova opcao para demonstrar busca binaria
    printf("| %-30s |\n", "0 - Sair");
    printf("+------------------------------+\n");
}

// Retorna 1 se o email for valido e unico, 0 caso contrario
// O parametro 'email_atual_piloto' e usado durante a edicao para permitir que o email seja o mesmo do piloto sendo editado
int validar_email(const char *email, const char *email_atual_piloto) {
    // 1. Verificar se contem '@'
    if (strchr(email, '@') == NULL) {
        printf("Erro: O e-mail deve conter o caractere '@'.\n");
        return 0;
    }

    // 2. Verificar o comprimento (menor que 80)
    if (strlen(email) >= MAX_CHAR_EMAIL) {
        printf("Erro: O e-mail deve ter menos de %d caracteres.\n", MAX_CHAR_EMAIL);
        return 0;
    }

    // 3. Verificar unicidade (com outros pilotos, desconsiderando o proprio se em edicao)
    NoPiloto *atual = lista_pilotos_head;
    while (atual != NULL) {
        // Se o email já existe e não é o email do piloto que está sendo editado (se email_atual_piloto for fornecido)
        if (strcmp(atual->dados.email, email) == 0) {
            if (email_atual_piloto == NULL || strcmp(email_atual_piloto, email) != 0) {
                 printf("Erro: Ja existe um piloto cadastrado com este e-mail.\n");
                return 0;
            }
        }
        atual = atual->proximo;
    }
    
    return 1; // E-mail valido e unico
}