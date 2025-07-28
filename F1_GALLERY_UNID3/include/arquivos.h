#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "pilotos_estatisticas.h" // Para as structs de dados e as variaveis globais

// Constantes para nomes de arquivos
#define NOME_ARQUIVO_PILOTOS "pilotos.txt"
#define NOME_ARQUIVO_ESTATISTICAS "estatisticas.txt"

// Prototipos das funcoes de arquivo
void carregar_pilotos_do_arquivo();
void salvar_pilotos_no_arquivo();
void carregar_estatisticas_do_arquivo();
void salvar_estatisticas_no_arquivo();

#endif // ARQUIVOS_H