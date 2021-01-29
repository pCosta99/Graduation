#ifndef ___LIST_H___
#define ___LIST_H___

/**
@file list.h
Definição das funções relativas ao histórico
*/

/**
Função que adiciona um elemento à lista ligada presente no estado

@param e O estado onde se encontra a lista
@param X A coordenada X de onde ocorreu a jogada
@param Y A coordenada Y de onde ocorreu a jogada
@param val O valor da jogada
@returns O estado inicial com a alteração na lista
*/
ESTADO push(ESTADO e, int X, int Y, char val);

/**
Função que retrocede uma jogada

@param e O estado inicial
@returns O estado após retroceder uma jogada
*/
ESTADO pop(ESTADO e);

/**
Função que escreve a lista para um ficheiro

@param e O estado que contém a lista
*/
void histToFile(ESTADO e);

/**
Função que lê uma lista para o estado de um ficheiro

@param e O estado onde está a lista por ler
@returns O estado com a nova lista
*/
ESTADO fileToHist(ESTADO e);

/**
Função que inverte uma lista

@param l A lista
@returns A lista invertida
*/
LISTA revL (LISTA l);

#endif