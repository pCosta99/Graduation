#ifndef ___LER_ESTADO_H___
#define ___LER_ESTADO_H___

/**
@file ler_estado.h
Definição das funções que preparam o estado para o resto da execução do programa e das funções que dispõe menus
*/

/**
Função que lê toda a informação relevante relativa ao estado

@returns O estado
*/
ESTADO ler_estado();

/**
Função que lê o nome do ficheiro a partir da query_string

@param query_string A query_string
@param filename Onde escrever o nome do ficheiro
*/
void get_file_name(char *query_string, char *filename);

/**
Função que lê a ação a partir da query_string

@param query_string A query_string
@param action Onde escrever a ação
*/
void get_action(char *query_string, char *action);

/**
Função que lê o nome do utilizador a partir da query_string

@param query_string A query_string
@param user Onde escrever o nome do utilizador
*/
void get_user(char *query_string, char *user);

/**
Função que dá display do menu inicial 
*/
void inicializar();

/**
Função que dá display do menu de vitória

@param e O estado
*/
void win(ESTADO e);

/**
Função que dá display ao menu de login inicial
*/
void choose_user();

/**
Função que avisa o utilizador que tentou carregar um jogo inválido
*/

void invalidFile();
#endif