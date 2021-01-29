#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"
#include "ajuda.h"
#include "resolveEstado.h"
#include "check.h"

#define MAX_BUFFER		10240

ESTADO invert (ESTADO e) {
	ESTADO novo = e;
	int X, Y;
	for (X = 0; X < e.num_lins; X++) 
		for (Y = 0; Y < e.num_cols; Y++) {
			if (e.grelha[X][Y] == SOL_X) novo.grelha[X][Y] = SOL_O;
			else novo.grelha[X][Y] = SOL_X;
		}
	return novo;
}

ESTADO rotate (ESTADO e) {
	ESTADO novo = e;
	int X, Y, Z;
	for (Y = (e.num_cols-1), Z = 0; Y >= 0 && Z < e.num_lins; Y--, Z++) 
		for (X = 0; X < e.num_lins; X++) {
			novo.grelha[X][Y] = e.grelha[Z][X]; 
		}
	return novo;
}

void colocaXO (ESTADO *e, int putX) {
	int X, Y;
	for (X = 0; X < e->num_lins; X++) 
		 for (Y = 0; Y < e->num_cols; Y++) {
		 	if (e->grelha[X][Y] == VAZIA) {
		 		if (putX) e->grelha[X][Y] = SOL_X;
		 		else e->grelha[X][Y] = SOL_O;
		 		return;
		 	}
		 }
}

void solveVazioRet (ESTADO e, ESTADO *solutions, int *n_sols) {
	ESTADO novo = e;
	int X;
	int control = 0;
	int k = 0;
	while (control < 4) {
		if (control == 0) {
			for (X = 0; X < e.num_lins; X++) {
				while (k < e.num_cols) {
					if (X%2) {
						novo.grelha[X][k++] = SOL_X;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
					}
					else {
						novo.grelha[X][k++] = SOL_O;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
					}
				}
				k = 0;
			}
		}
		else if (control == 1) {
			for (X = 0; X < e.num_cols; X++) {
				while (k < e.num_lins) {
					if (X%2) {
						novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
					}
					else {
						novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
					}
				}
				k = 0;
			}
		}
		else if (control == 2) {
			for (X = 0; X < e.num_cols; X++) {
				while (k < e.num_lins) {
					if (X%2) {
						novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
					}
					else {
						novo.grelha[k++][X] = SOL_O;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_X;
						if (k < e.num_lins) novo.grelha[k++][X] = SOL_O;
					}
				}
				k = 0;
			}
		}
		else if (control == 3) {
			for (X = 0; X < e.num_lins; X++) {
				while (k < e.num_cols) {
					if (X%2) {
						novo.grelha[X][k++] = SOL_X;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
						if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
					}
					else {
						novo.grelha[X][k++] = SOL_O;
						if (k < e.num_lins) novo.grelha[X][k++] = SOL_X;
						if (k < e.num_lins) novo.grelha[X][k++] = SOL_X;
						if (k < e.num_lins) novo.grelha[X][k++] = SOL_O;
					}
				}
				k = 0;
			}
		}
		control++;
		solutions[(*n_sols)++] = novo;
		solutions[(*n_sols)++] = invert(novo);
		novo = e;
	}
}

void solveVazioQuadrado (ESTADO e, ESTADO *solutions, int *n_sols) {
	ESTADO novo = e;
	int ang = 0;
	int k = 0;
	int X = 0;
	while (X < e.num_lins) {
		while (k < e.num_cols) {
			if (X%2) {
				novo.grelha[X][k++] = SOL_X;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
			}
			else {
				novo.grelha[X][k++] = SOL_O;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_O;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
				if (k < e.num_cols) novo.grelha[X][k++] = SOL_X;
			}
		}
		k = 0;
		X++;
	}

	solutions[(*n_sols)++] = novo;
	solutions[(*n_sols)++] = invert(novo);

	while (ang < 270) {
		solutions[(*n_sols)++] = rotate(novo);
		novo = rotate(novo);
		solutions[(*n_sols)++] = invert(novo);
		ang += 90;
	}
}

void resolveRecAll (ESTADO *e, ESTADO *solutions, int *n_sols) {
	int lins = e->num_lins;
	int cols = e->num_cols;
	ESTADO ant = *e;
	ESTADO current = ajuda(*e, 0, 0);
	ESTADO nextX;
	ESTADO nextO;
	if (*n_sols > 50) *n_sols = 0;

	if (lins >= 5 && cols >= 5 && checkVazio(*e)) {
		if (lins == cols) {solveVazioQuadrado(*e, solutions, n_sols); return;} 
		else {solveVazioRet(*e, solutions, n_sols); return;}
	}

	while (!estados_iguais(ant, current)) {
		ant = current;
		current = ajuda(current, 0, 0);
	}

	nextX = nextO = current;
	colocaXO(&nextX, 1);
	colocaXO(&nextO, 0);

	if (check_win(current)) {solutions[*n_sols] = current; *n_sols += 1; return;}
	else if (!check_estado(nextX)) return;
	else if (!check_estado(nextO)) return;

	resolveRecAll(&nextX, solutions, n_sols);
	resolveRecAll(&nextO, solutions, n_sols);
}