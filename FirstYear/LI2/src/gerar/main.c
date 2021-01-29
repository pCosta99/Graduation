/**
@file main.c
Esqueleto do programa
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "estado.h"
#include "gerar.h"
#include "resolveEstado.h"
#include "check.h"

int main(int argc, char *argv[]) {
	srand(clock());
	ESTADO e = {0};
	ESTADO *solutions = malloc(sizeof(ESTADO)*50);

	int dif = atoi(argv[1]);
	e.num_lins = atoi(argv[2]);
	e.num_cols = atoi(argv[3]);

	if (dif > 2 || dif < 1) {printf("Grau de dificuldade inválido.\n"); return 0;}

	gera(&e, solutions, dif);

	imprime(e);

	(void) argc;

	free(solutions);

	return 0;
}