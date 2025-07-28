#ifndef UTEIS_H
#define UTEIS_H

// Inclua outros cabeçalhos que as funções em uteis.c precisem, se houver
#include "pilotos_estatisticas.h" // Para acessar lista_pilotos_head e as structs de pilotos

// Constantes para login
#define USERNAME "admin"
#define PASSWORD "12345"

// Prototipos das funcoes uteis
void realizar_login();
void exibir_menu();
// Adicionei um parametro 'email_atual_piloto' para permitir que o email seja o mesmo do piloto sendo editado
int validar_email(const char *email, const char *email_atual_piloto); 

#endif // UTEIS_H