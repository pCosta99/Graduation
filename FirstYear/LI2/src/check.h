#ifndef ___CHECK_H___
#define ___CHECK_H___

/**
@file check.h
Definição das funções que verificam diversas propriedades do estado
*/

/**
Função que verifica se uma casa pertence a um certo conjunto de valores
@param e O estado
@param X Coordenada X da casa a alterar
@param Y Coordenada Y da casa a alterar
@param control Controla qual o conjunto a verificar
@param symbol Controla qual o símbolo a verificar (X ou O)
@returns Retorna 1 se for pertencer ao conjunto em causa, 0 caso contrário
*/
int check_grelha (ESTADO e, int X, int Y, int control, char symbol);

/**
Função que altera uma casa para o respetivo valor mas como inválida

@param e O estado
@param X Coordenada X da casa a alterar
@param Y Coordenada Y da casa a alterar
@param valor Valor da casa a alterar
*/
void changeToInv (ESTADO *e, int X, int Y, char valor);

/**
Função que verifica a validade de um conjunto de três casas sobre um determinado vetor

@param e O estado
@param a Coordenada X do vetor
@param b Coordenada Y do vetor
@param control Controla se é suposto alterar as casa em causa para inválidas ou não
@returns 0 se o estado é inválido (sobre este vetor), 1 caso contrário 
*/
int check_linha(ESTADO *e, int a, int b, int control);

/**
Função que verifica se um estado é válido

@param e O estado
@returns 1 se for válido, 0 caso contrário
*/
int check_estado(ESTADO e);

/**
Função que verifica se dois estados são iguais e, no caso de não serem, coloca inválidas no primeiro

@param e Primeiro estado
@param novo Segundo estado
@returns O estado com as casas invertidas ou o inicial, consoante o caso 
*/
ESTADO compara_estados(ESTADO e, ESTADO novo); 

/**
Função que verifica se o estado a validade do estado e a presença de casas inválidas no mesmo

@param e O estado a verificar
@returns 1 se o estado tem casas inválidas e é inválido, 0 se o estado tem inválidas e é inválido
*/
int check_inv(ESTADO e); 

/**
Função que verifica se um estado está completo (se é uma vitória)

@param e O estado a verificar
@returns 1 se o estado está completo, 0 caso contrário
*/
int check_win(ESTADO e);

/**
Função que verifica se um tabuleiro está completamente preenchido, sem casas inválidas

@param e O estado a verificar
@returns 1 se o estado não está completo, 0 caso contrário
*/
int check_full_tab (ESTADO e);

/**
Função que verifica se dois estados são iguais

@param a Primeiro estado
@param b Segundo estado
@returns 1 se são iguais, 0 caso contrário
*/
int estados_iguais (ESTADO e, ESTADO b);

/** 
Função que altera as casas inválidas para esse mesmo valor

@param e O estado a alterar
@returns O estado alterado
*/
ESTADO turn_inv (ESTADO e);

/**
Função que altera todas as casas inválidas para casas válidas

@param e O estado a alterar
@returns O estado alterado
*/
ESTADO turn_valid (ESTADO e);

/** 
Função que verifica se um estado está completamente vazio

@param e O estado a verificar
@returns 1 se está completamente vazio, 0 caso contrário
*/
int checkVazio (ESTADO e);

#endif