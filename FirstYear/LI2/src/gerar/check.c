#include "estado.h"
#include "check.h"
#include "ajuda.h"

int check_grelha (ESTADO e, int X, int Y, int control, char symbol) {
	if ((e.grelha[X][Y] == SOL_X || e.grelha[X][Y] == FIXO_X || e.grelha[X][Y] == INV_X) && control == 1 && symbol == 'X') return 1;
	else if ((e.grelha[X][Y] == SOL_X || e.grelha[X][Y] == VAZIA || e.grelha[X][Y] == INV_X) && control == 0 && symbol == 'X') return 1;
	else if ((e.grelha[X][Y] == SOL_O || e.grelha[X][Y] == FIXO_O || e.grelha[X][Y] == INV_O) && control == 1 && symbol == 'O') return 1;
	else if ((e.grelha[X][Y] == SOL_O || e.grelha[X][Y] == VAZIA || e.grelha[X][Y] == INV_O) && control == 0 && symbol == 'O') return 1;
	else return 0;
}

void changeToInv (ESTADO *e, int X, int Y, char valor) {
	if (e->grelha[X][Y] != FIXO_X && valor == 'X') e->grelha[X][Y] = INV_X;
	if (e->grelha[X][Y] != FIXO_O && valor == 'O') e->grelha[X][Y] = INV_O;
}

int check_linha (ESTADO *e, int a, int b, int control) {
	int X, Y;
	int i = a/2;
	int j = b/2;
	for (X = 0; X < e->num_lins; X++) 
		for (Y = 0; Y < e->num_cols; Y++) {
			if ((X+a) >= 0 && (Y+b) >= 0 && (X+a) < e->num_lins && (Y+b) < e->num_cols && (X+i) >= 0 && (Y+j) >= 0 && (X+i) < e->num_lins && (Y+j) < e->num_cols){
				if (check_grelha(*e, X, Y, 1, 'X') && check_grelha(*e, X+a, Y+b, 1, 'X') && check_grelha(*e, X+i, Y+j, 1, 'X')) {
					if (control) return 0;
					else {
						changeToInv(e, X, Y, 'X');
						changeToInv(e, X+a, Y+b, 'X');
						changeToInv(e, X+i, Y+j, 'X');
						return 0;
					}
				}
				if (check_grelha(*e, X, Y, 1, 'O') && check_grelha(*e, X+a, Y+b, 1, 'O') && check_grelha(*e, X+i, Y+j, 1, 'O')) {
					if (control) return 0;
					else {
						changeToInv(e, X, Y, 'O');
						changeToInv(e, X+a, Y+b, 'O');
						changeToInv(e, X+i, Y+j, 'O');
						return 0;
					} 
				}
			}
		}
	return 1;
}

int check_estado (ESTADO e) {
	return (check_linha(&e, 2, 2, 1) && check_linha(&e, 0, 2, 1) && check_linha(&e, 2, 0, 1) && check_linha(&e, 2, -2, 1));
}

ESTADO compara_estados (ESTADO e, ESTADO novo) {
	int i, j;
	int k = 0;
	for (i = 0; i < e.num_lins; i++) {
		for (j = 0; j < e.num_cols; j++) {
			if (e.grelha[i][j] != novo.grelha[i][j]) {
				k = 1;
			} 
		}
	}

	if (k == 1) {
		novo = turn_inv(e);
	}

	return novo;
}
//1 se o estado tem invalidas e é valido ou se não tem invalidas; 0 se o estado tem invalidas e é invalido
int check_inv (ESTADO e) {
	int i, j;
	int k = 0;

	for (i = 0; i < e.num_lins; i++)
		for (j = 0; j < e.num_cols; j++) {
			if (e.grelha[i][j] == INV_X || e.grelha[i][j] == INV_O) {
				k = 1;
			}
		}
	if (k == 1 && check_estado(e)) return 1; 
	else if (k == 1 && !check_estado(e)) return 0;
	else return 2;
}

int check_win (ESTADO e) {
	if (check_estado(e) && !check_full_tab(e)) return 1;
	else if (e.win == 1) return 1;
	else return 0;
}

int check_full_tab (ESTADO e) {
	int i, j;
	for (i = 0; i < e.num_lins; i++) 
		for (j = 0; j < e.num_cols; j++) {
			if (e.grelha[i][j] == VAZIA || e.grelha[i][j] == INV_X || e.grelha[i][j] == INV_O) return 1;
		}
	return 0;
}
// 0 se são diferentes 1 se são iguais
int estados_iguais (ESTADO a, ESTADO b) {
	int i, j;
	if (a.type != b.type) return 0;
	else if (a.num_cols != b.num_cols) return 0;
	else if (a.num_lins != b.num_lins) return 0;
	
	for (i = 0; i < a.num_lins; i++)
		for (j = 0; j < a.num_cols; j++) {
			if (a.grelha[i][j] != b.grelha[i][j]) return 0;
		}

	return 1;
}

ESTADO turn_inv (ESTADO e) {
	check_linha(&e, 2, 2, 0); 
	check_linha(&e, 0, 2, 0); 
	check_linha(&e, 2, 0, 0); 
	check_linha(&e, 2, -2, 0);

	return e;
}

ESTADO turn_valid (ESTADO e) {
	int i, j;
	for (i = 0; i < e.num_lins; i++) 
		for (j = 0; j < e.num_cols; j++) {
			if (e.grelha[i][j] == INV_X) e.grelha[i][j] = SOL_X;
			if (e.grelha[i][j] == INV_O) e.grelha[i][j] = SOL_O;
		}
	return e;
}

int checkVazio (ESTADO e) {
	int X, Y;
	for (X = 0; X < e.num_lins; X++) 
		for (Y = 0; Y < e.num_cols; Y++) {
			if (e.grelha[X][Y] != VAZIA) return 0;
		}
	return 1;
}