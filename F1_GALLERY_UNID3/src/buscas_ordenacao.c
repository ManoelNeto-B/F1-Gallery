#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/arquivos.h"
#include "../include/pilotos_estatisticas.h" // Para acessar lista_pilotos_head e structs


// --- Implementacao de Quicksort para Lista Encadeada ---

// Funcao auxiliar para encontrar o ultimo no da lista
NoPiloto* get_tail(NoPiloto *node) {
    while (node != NULL && node->proximo != NULL) {
        node = node->proximo;
    }
    return node;
}

// Funcao auxiliar para particionar a lista para o Quicksort
// Retorna o pivo depois da particao e atualiza new_head e new_end
NoPiloto* partition_quicksort(NoPiloto *head, NoPiloto *end, NoPiloto **new_head, NoPiloto **new_end) {
    NoPiloto *pivot = end;
    NoPiloto *prev = NULL, *cur = head, *tail = pivot;

    // Percorre a lista e move os elementos menores que o pivo para a esquerda, e maiores para a direita
    while (cur != pivot) {
        if (strcmp(cur->dados.nome, pivot->dados.nome) < 0) { // Menor que o pivo
            if (*new_head == NULL) { // Se for o primeiro elemento menor, ele é a nova cabeça
                *new_head = cur;
            }
            prev = cur;
            cur = cur->proximo;
        } else { // Maior ou igual ao pivo
            if (prev) { // Se não for o primeiro elemento, remove o atual da sua posição
                prev->proximo = cur->proximo;
            }
            NoPiloto *temp = cur->proximo; // Guarda o proximo do atual
            cur->proximo = NULL; // Desconecta o atual
            tail->proximo = cur; // Adiciona o atual no final da sub-lista 'maior'
            tail = cur; // Atualiza o final da sub-lista 'maior'
            cur = temp; // Avança para o proximo elemento original
        }
    }

    if (*new_head == NULL) { // Se todos os elementos forem maiores que o pivo, o pivo é a nova cabeça
        *new_head = pivot;
    }
    *new_end = tail; // O final da lista particionada
    return pivot;
}

// Implementacao do Quicksort recursivo para lista encadeada
// Retorna a nova cabeca da lista apos a ordenacao
NoPiloto* _quicksort_recursivo(NoPiloto *head, NoPiloto *end) {
    if (!head || head == end) { // Caso base: lista vazia ou com um único elemento
        return head;
    }

    NoPiloto *new_head = NULL, *new_end = NULL;
    NoPiloto *pivot = partition_quicksort(head, end, &new_head, &new_end);

    // Se a sub-lista da esquerda não estiver vazia (ou seja, new_head não é o próprio pivot)
    if (new_head != pivot) {
        NoPiloto *temp = new_head;
        while (temp->proximo != pivot) { // Encontra o último elemento da sub-lista da esquerda
            temp = temp->proximo;
        }
        temp->proximo = NULL; // Separa a sub-lista da esquerda do resto
        new_head = _quicksort_recursivo(new_head, temp); // Ordena recursivamente a sub-lista da esquerda

        // Encontra o ultimo no da sub-lista ordenada da esquerda
        temp = get_tail(new_head);
        temp->proximo = pivot; // Conecta a sub-lista da esquerda com o pivo
    }

    // Ordena recursivamente a sub-lista da direita e conecta com o pivo
    pivot->proximo = _quicksort_recursivo(pivot->proximo, new_end);

    return new_head; // Retorna a nova cabeça da lista ordenada
}

// Funcao principal para iniciar o Quicksort na lista global
void quicksort_pilotos(NoPiloto **head_ref) {
    *head_ref = _quicksort_recursivo(*head_ref, get_tail(*head_ref));
}

void ordenar_pilotos_por_nome() {
    if (lista_pilotos_head == NULL || lista_pilotos_head->proximo == NULL) {
        printf("\nNao ha pilotos suficientes para ordenar (minimo 2).\n");
        return;
    }

    printf("\nOrdenando pilotos por nome (Quicksort)...\n");
    quicksort_pilotos(&lista_pilotos_head); // Passa o endereço do ponteiro da cabeça
    printf("Pilotos ordenados com sucesso!\n");
    // Opcional: chamar listar_pilotos() aqui para mostrar o resultado imediato
    
}

NoPiloto* buscar_piloto_por_nome_binaria(NoPiloto *head, const char *nome_busca) {
    if (head == NULL) {
        return NULL;
    }

    // 1. Contar o numero de elementos na lista
    int count = 0;
    NoPiloto *current = head;
    while (current != NULL) {
        count++;
        current = current->proximo;
    }

    if (count == 0) return NULL;

    // 2. Copiar os ponteiros dos nos para um array temporario
    NoPiloto **array_temp = (NoPiloto **)malloc(count * sizeof(NoPiloto *));
    if (array_temp == NULL) {
        printf("Erro: Falha ao alocar memoria para busca binaria.\n");
        return NULL;
    }

    current = head;
    for (int i = 0; i < count; i++) {
        array_temp[i] = current;
        current = current->proximo;
    }


    // 3. Realizar busca binaria no array de ponteiros
    int low = 0, high = count - 1;
    NoPiloto *found_piloto = NULL;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(array_temp[mid]->dados.nome, nome_busca);

        if (cmp == 0) {
            found_piloto = array_temp[mid];
            break;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    free(array_temp); // Liberar a memoria do array temporario
    return found_piloto;
}

void buscar_piloto_por_nome_binaria_menu() {
    printf("\n--- Buscar Piloto por Nome (Busca Binaria) ---\n");
    if (lista_pilotos_head == NULL) {
        printf("Nenhum piloto cadastrado para buscar.\n");
        return;
    }

    printf("AVISO: Para a busca binaria funcionar corretamente, os pilotos devem estar ordenados por nome.\n");
    printf("Considere usar a opcao 11 (Ordenar Pilotos por Nome) antes de realizar esta busca.\n\n");


    char nome_busca[MAX_CHAR_NOME];
    printf("Digite o nome do piloto que deseja buscar: ");
    fgets(nome_busca, sizeof(nome_busca), stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    NoPiloto *piloto_encontrado = buscar_piloto_por_nome_binaria(lista_pilotos_head, nome_busca);

    if (piloto_encontrado != NULL) {
        printf("\nPiloto encontrado!\n");
        printf("ID: %d\n", piloto_encontrado->dados.id);
        printf("Nome: %s\n", piloto_encontrado->dados.nome);
        printf("Origem: %s\n", piloto_encontrado->dados.origem);
        printf("Apelido: %s\n", piloto_encontrado->dados.apelido);
        printf("Equipe: %s\n", piloto_encontrado->dados.equipe);
        printf("E-mail: %s\n", piloto_encontrado->dados.email);

        NoEstatistica *estatistica_encontrada = buscar_estatisticas_por_piloto_id_lista(piloto_encontrado->dados.id);
        if (estatistica_encontrada != NULL) {
            printf("  --- Estatisticas ---\n");
            printf("  GPs: %d\n", estatistica_encontrada->dados.grandes_premios);
            printf("  Vitorias: %d\n", estatistica_encontrada->dados.vitorias);
            printf("  Poles: %d\n", estatistica_encontrada->dados.poles);
            printf("  Podios: %d\n", estatistica_encontrada->dados.podios);
            printf("  Melhores Voltas: %d\n", estatistica_encontrada->dados.mvoltas);
            printf("  Titulos Mundiais: %d\n", estatistica_encontrada->dados.mundiv);
        }
    } else {
        printf("Piloto '%s' nao encontrado.\n", nome_busca);
    }
}