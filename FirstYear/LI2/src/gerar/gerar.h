#ifndef ___GERAR_H___
#define ___GERAR_H___

/**
@file gerar.h
Definição das funções que geram estados aleatórios
*/

/**
Função que imprime o estado gerado no terminal

@param e O estado a imprimir
*/
void imprime (ESTADO e);

/**
Função que coloca todas as casas de um estado a vazio

@param e O estado a alterar
*/
void createState (ESTADO *e);

/**
Função que gera um número aleatório entre dois valores

@param lim_inf O limite inferior do número a ser gerado
@param lim_sup O limite superior do número a ser gerado
*/
int geraNumero (int lim_inf, int lim_sup);

/**
Função que indica o número de dígitos que um inteiro tem

@param n O inteiro
@returns O número de dígitos do inteiro
*/
int digits (int n);

/**
Função que gera uma casa aleatória

@param e O estado
@param str String onde é colocada a casa gerada
*/
void geraCell (ESTADO e, char *str);

/**
Função que escolhe uma casa aleatória dentro de várias

@param e O estado
@param random A string onde é colocada a casa escolhida
*/
void chooseCell (ESTADO e, char *random);

/**
Função que altera todas as casas para o seu valor fixo correspondente 

@param e O estado a alterar
*/
void changeToFixo (ESTADO *e);

/**
Função que verifica se é possível resolver um estado unicamente através de ajudas lógicas

@param e O estado a verificar
@returns 1 se for possível, 0 caso contrário
*/
int resComAjudas (ESTADO e);

/**
Função que gera um estado aleatório de dificuldade fácil a partir de um estado vazio

@param e O estado inicial
@param solutions Todas as soluções possíveis do estado
@param n_sols O número de soluções do estado
*/
void geraFacil (ESTADO *e, ESTADO *solutions, int *n_sols);

/**
Função que altera uma casa em específico para o seu valor fixo correspondente

@param e O estado a alterar
@param x Coordenada X da casa a alterar
@param y Coordenada Y da casa a alterar
*/
void changeToFixov2 (ESTADO *e, int X, int Y);

/**
Função que altera uma casa dum estado para o valor da mesma da casa de um outro estado

@param e O estado a alterar
@param copy O estado de onde se copia a casa em causa
@param random String que contém a casa a copiar
*/
void alteraEstado (ESTADO *e, ESTADO *copy, char *random);

/**
Função que gera um estado aleatório

@param e O estado
@param solutions Todas as soluções possíveis do estado
@param n_sols O número de soluções do estado
*/
void geraRandom (ESTADO *e, ESTADO *solutions, int n_sols);

/**
Função que gera um estado aleatório de dificuldade difícil com base num estado vazio e numa resolução do mesmo

@param e O estado
@param solutions Todas as soluções possíveis do estado
@param n_sols O número de soluções do estado
*/
void geraDificil (ESTADO *e, ESTADO *solutions, int *n_sols); 

/**
Função que redireciona o estado a ser gerado consoante a dificuldade desejada

@param e O estado
@param solutions Todas as soluções possíveis do estado
@param dif Nível da dificuldade
*/
void gera (ESTADO *e, ESTADO *solutions, int dif);

#endif