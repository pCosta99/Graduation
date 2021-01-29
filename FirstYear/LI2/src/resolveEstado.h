#ifndef ___RESOLVE_ESTADO_H___
#define ___RESOLVE_ESTADO_H___

/**
@file resolveEstado.h
Definição das funções que resolvem um estado
*/

/**
Função que inverte um estado, isto é, transforma um X numa O

@param e O estado a inverter
@returns O estado invertido
*/
ESTADO invert (ESTADO e);

/**
Função que roda, por 90º, um estado

@param e O estado a rodar
@returns O estado alterado
*/
ESTADO rotate (ESTADO e);

/**
Função que coloca um X ou uma O na primeira casa vazia

@param e O estado onde será colocado o X ou a O
@param putX Variável de control. Se for 1 coloca-se um X, caso contrário coloca-se um O
*/
void colocaXO (ESTADO *e, int putX);

/**
Função que resolve um estado vazio retangular

@param e O estado a ser resolvido
@param solutions Array de estados onde serão colocadas todas as soluções
@param n_sols O número de soluções para o estado em causa
*/
void solveVazioRet (ESTADO e, ESTADO *solutions, int *n_sols);

/**
Função que resolve um estado vazio quadrado

@param e O estado a ser resolvido
@param solutions Array de estados onde serão colocadas todas as soluções
@param n_sols O número de soluções para o estado em causa
*/
void solveVazioQuadrado (ESTADO e, ESTADO *solutions, int *n_sols);

/**
Função que resolve qualquer estado através de ajudas lógicas e recursividade

@param e O estado a ser resolvido
@param solutions Array de estados onde serão colocadas todas as soluções
@param n_sols O número de soluções para o estado em causa
*/
void resolveRecAll (ESTADO *e, ESTADO *solutions, int *n_sols);

#endif