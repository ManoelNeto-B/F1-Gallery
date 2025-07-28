#ifndef BUSCAS_ORDENACAO_H
#define BUSCAS_ORDENACAO_H

#include "pilotos_estatisticas.h" // Para NoPiloto e Piloto

// Prototipos para ordenacao
void quicksort_pilotos(NoPiloto **head_ref); // Função principal do quicksort para a lista
void ordenar_pilotos_por_nome(); // Função para ser chamada do menu

// Prototipos para busca binaria
// Retorna o piloto encontrado ou NULL
NoPiloto* buscar_piloto_por_nome_binaria(NoPiloto *head, const char *nome_busca); 
void buscar_piloto_por_nome_binaria_menu(); // Função que será chamada do menu para demonstrar

#endif // BUSCAS_ORDENACAO_H