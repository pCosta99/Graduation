#ifndef ___FILE_H___
#define ___FILE_H___

/**
@file file.h
Definição das funções que manipulam ficheiros
*/

/** 
Função que indica o número de estados gerados aleatoreamente até ao momento

@param e O estado em causa
@returns Número de estados gerados
*/
int numeroGerados(ESTADO e);

/**
Função que escreve o estado num ficheiro 

@param e O estado a ser escrito
*/
int estadotofile(ESTADO e);

/**
Função que lê o estado de um ficheiro

@param e O estado atual
@returns O estado presente no ficheiro em causa
*/
ESTADO get_estado_from_file(ESTADO e);

/**
Função que lê o user do ficheiro onde este está escrito

@param e O estado atual
@returns O estado com o user correto
*/
ESTADO get_user_from_file (ESTADO e);

/**
Função que esreve o user para um ficheiro

@param e O estado
*/
void write_user_to_file (ESTADO e);

/**
Função que copia um estado de um ficheiro para outro

@param source Localização do ficheiro para ler
@param destiny Localização do ficheiro para onde escrever
*/
void copy_estados(char *source, char *destiny); 

/**
Função que lê uma macro, um ficheiro .css

@param source Localização do ficheiro 
*/
void readMacro (char *source);

#endif