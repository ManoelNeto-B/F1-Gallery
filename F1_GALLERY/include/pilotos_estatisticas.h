#ifndef PILOTOS_ESTATISTICAS_H
#define PILOTOS_ESTATISTICAS_H

// --- Constantes ---
#define MAX_CHAR_NOME 70
#define MAX_CHAR_ORIGEM 20
#define MAX_CHAR_APELIDO 20
#define MAX_CHAR_EQUIPE 20
#define MAX_CHAR_EMAIL 80

// --- Estruturas de Dados ---

typedef struct {
    int id;
    char nome[MAX_CHAR_NOME];
    char origem[MAX_CHAR_ORIGEM];
    char apelido[MAX_CHAR_APELIDO];
    char equipe[MAX_CHAR_EQUIPE];
    char email[MAX_CHAR_EMAIL];
} Piloto; // Renomeado de Pilotos para Piloto no singular

typedef struct {
    int piloto_id;
    int grandes_premios;
    int vitorias;
    int poles;
    int podios;
    int mvoltas;
    int mundiv;
} Estatistica; // Renomeado de Cestatisticas para Estatistica no singular

// NÓS DAS LISTAS ENCADEADAS
typedef struct NoPiloto {
    Piloto dados;
    struct NoPiloto *proximo;
} NoPiloto;

typedef struct NoEstatistica {
    Estatistica dados;
    struct NoEstatistica *proximo;
} NoEstatistica;

// Variáveis globais para as cabeças das listas (declaradas como extern aqui)
extern NoPiloto *lista_pilotos_head;
extern NoEstatistica *lista_estatisticas_head;
extern int proximo_piloto_id; // Para gerar IDs sequenciais

// --- Prototipos de Funcoes de Manipulacao de Pilotos e Estatisticas ---
void adicionar_piloto(Piloto novo_piloto);
void Cadastro_Piloto();
void listar_pilotos();
NoPiloto* buscar_piloto_por_id_lista(int id_busca); // Retorna o nó, não o índice
void editar_piloto_menu(); // Funcao que sera chamada do main para edicao
void excluir_piloto_menu(); // Funcao que sera chamada do main para exclusao
void comparar_pilotos();
void exibir_campeoes(); // Nova implementacao de campeoes

void adicionar_estatistica(Estatistica nova_estatistica);
void Cadastro_Estatistica();
NoEstatistica* buscar_estatisticas_por_piloto_id_lista(int piloto_id_busca); // Retorna o nó

#endif // PILOTOS_ESTATISTICAS_H