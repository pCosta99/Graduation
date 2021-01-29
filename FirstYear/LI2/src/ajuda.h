#ifndef ___AJUDA_H___
#define ___AJUDA_H___

/**
@file ajuda.h 
Definição das funções que provêm uma ajuda lógica
*/

/**
Função que altera o estado numa certa casa através de um vetor fornecido, aquilo a que se poderia chamar uma ajuda lógica

@param e O estado
@param a Coordenada x do vetor
@param b Coordenada y do vetor
@param control Variável que controla se é uma ajuda no meio de duas casas ou em duas casas consecutivas
@param str String onde é escrito a casa onde a mudança ocorreu
@returns O novo estado
*/
ESTADO altera (ESTADO e, int a, int b, int control);

/**
Função que troca o valor de duas variáveis

@param a Primeira variável
@param b Segunda variável
*/
void swap (int *a, int *b);

/**
Dá uma ajuda ao utilizar todos os vetores possíveis
@param e O estado
@param controlList Variável de controlo relativa a escrever a ajuda no histórico
@param controlBloq Variável de controlo que coloca uma bloqueada no sítio onde seria dada a ajuda
@returns O estado  
*/
ESTADO ajuda (ESTADO e, int controlList, int controlBloq);

#endif