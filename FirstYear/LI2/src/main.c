/**
@file main.c
Esqueleto do programa
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "estado.h"
#include "ler_estado.h"
#include "file.h"

#define MAX_BUFFER		10240
/**
Função principal do programa
@returns Retorna 0 no final da execução
*/
int main() {
	ESTADO e;

	e = ler_estado();
	e = calc_novo_estado(e); 
	printEstado(e);
	if ((e.type && !e.control && !e.skip) || e.gerar) estadotofile(e);
	
	return 0;
}