/**
@file ajuda.c
Função chamada pelo botão de ajuda.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "estado.h"
#include "check.h"
#include "list.h"

ESTADO altera (ESTADO e, int a, int b, int control, char *str) {
	char jogada[10];
	int X, Y;
	int i;
	int j;
	if (control) {
		i = a/2;
		j = b/2;
	}
	else {
		i = a*2;
		j = b*2;
	}
	for (X = 0; X < e.num_lins; X++)
		for (Y = 0; Y < e.num_cols; Y++) {
			if ((X+a) <= e.num_lins && (Y+b) <= e.num_cols && (X+a) >= 0 && (Y+b) >= 0 && (X+i) <= e.num_lins && (Y+j) <= e.num_cols && (X+i) >= 0 && (Y+j) >= 0) {
				if (check_grelha(e, X, Y, 1, 'X') && check_grelha(e, X+a, Y+b, 1, 'X') && check_grelha(e, X+i, Y+j, 0, 'X')) {
					sprintf(jogada, "%d %d %c", X+i, Y+j, e.grelha[X+i][Y+j]);
					e.grelha[X+i][Y+j] = SOL_O;
					strcpy(str,jogada);
					return e;
				}
				if (check_grelha(e, X, Y, 1, 'O') && check_grelha(e, X+a, Y+b, 1, 'O') && check_grelha(e, X+i, Y+j, 0, 'O')) {
					sprintf(jogada, "%d %d %c", X+i, Y+j, e.grelha[X+i][Y+j]);
					e.grelha[X+i][Y+j] = SOL_X;
					strcpy(str,jogada);
					return e;
				}
			}
		}

	return e;
}

void swap (int *a, int *b) {
	int t;

	t = *b;
	*b = *a;
	*a = t;
}

ESTADO ajuda (ESTADO e, int controlList, int controlBloq) {
	ESTADO novo = e;
	char *str = malloc(sizeof(char)*6);
	int lin, col;
	char val;
	int k = 0;
	int change = 0;
	int a, b;
	int control = 0;
	int fator = 2;
	a = b = fator;
	novo = altera(e, a, b, fator-1, str);

	while (estados_iguais(e,novo) && (k != 4 || control != 2)) {
		if (k == 0) {
			a = -a;
			b = -b;
			k++;
		}

		else if (k == 1) {
				if (!change) {change = 1; b += fator;}
				else {swap(&a,&b); k++; change = 0;}
			}

		else if (k == 2) {
				if (!change) {change = 1; a += fator;}
				else {swap(&a,&b); k++; change = 0;}
			}

		else if (k == 3) {
				if (!change) {change = 1; a += fator;}
				else {swap(&a,&b); k++; change = 0;}
			}

		else if (k == 4 && !control) {
			a = b = fator = control = 1;
			novo = altera(e, a, b, fator-1, str);
			if (!check_estado(novo)) novo = e;
			k = 0;
		}

		else if (k == 4 && control) control = 2;

		if (control != 2) {
			if (!control) novo = altera(e, a, b, fator-1, str);
			else {
				novo = altera(e, a, b, fator-1, str);
				if (!check_estado(novo)) novo = e;
			}
		}

	}

	if (check_estado(novo) && controlList) {sscanf(str, "%d %d %c", &lin, &col, &val); novo = push(novo, lin, col, val); novo.hist->init = 1; return novo;}
	else if (check_estado(novo) && controlBloq && !estados_iguais(e,novo)) {sscanf(str, "%d %d %c", &lin, &col, &val); novo.grelha[lin][col] = BLOQUEADA; return novo;}
	else  if (check_estado(novo)) return novo;
	else return e;
}


