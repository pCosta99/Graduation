#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

#include "estado.h"
#include "resolveEstado.h"
#include "ajuda.h"
#include "check.h"

void imprime (ESTADO e) {
	int X, Y;
	printf("%d %d\n", e.num_lins, e.num_cols);
	for (X = 0; X < e.num_lins; X++) {
		for (Y = 0; Y < e.num_cols; Y++) {
			if (e.grelha[X][Y] == VAZIA) printf(".");
			else if (e.grelha[X][Y] == FIXO_X) printf("X");
			else if (e.grelha[X][Y] == FIXO_O) printf("O");
			else if (e.grelha[X][Y] == BLOQUEADA) printf("#");
		}
		printf("\n");
	}	
}

void createState (ESTADO *e) {
	for (int X = 0; X < e->num_lins; X++)
		for (int Y = 0; Y < e->num_cols; Y++) 
			e->grelha[X][Y] = VAZIA;
}

int geraNumero (int lim_inf, int lim_sup) {
	return (lim_inf + ((rand() % (lim_sup - lim_inf + 1))));
}

int digits (int n) {
    int r = 1;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

void geraCell (ESTADO e, char *str) {
	int lin, col, tam;
	lin = geraNumero(0, e.num_lins-1);
	col = geraNumero(0, e.num_cols-1);
	tam = e.num_cols+e.num_lins+1;
	char casa[tam];
	sprintf(casa, "%d %d", lin, col);
	casa[tam] = '\0';
	strcpy(str, casa);
}

void chooseCell (ESTADO e, char *random) {
	char array[5][10];
	char str[10];
	int i = 0;
	int len;

	while (i < 5) {
		geraCell(e, str);
		strcpy(array[i],str);
		len = strlen(str);
		array[i][len] = '\0';
		i++;
	}

	strcpy(random, array[geraNumero(1,5)]);
}

void changeToFixo (ESTADO *e) {
	int X, Y;
	for (X = 0; X < e->num_lins; X++)
		for (Y = 0; Y < e->num_cols; Y++) {
			if (e->grelha[X][Y] == SOL_X) e->grelha[X][Y] = FIXO_X;
			else if (e->grelha[X][Y] == SOL_O) e->grelha[X][Y] = FIXO_O;
		}
}

int resComAjudas (ESTADO e) {
	ESTADO aux = e;
	ESTADO novo = ajuda(e, 0, 0);
	while (!check_win(novo)) {
		if (estados_iguais(aux,novo)) return 0;
		aux = novo;
		novo = ajuda(novo, 0, 0);
	}
	return 1;
}

void geraFacil (ESTADO *e, ESTADO *solutions, int *n_sols) {
	char random[10];
	int control = 0;
	int lin, col;
	int cells = e->num_lins * e->num_cols;
	int backupCells = cells;
	int rem = 0.6 * cells;
	int ind;
	if (*n_sols) ind = geraNumero(0, *n_sols -1);
	else ind = 0;
	*e = solutions[ind];
	ESTADO novo = *e;
	while (rem < cells) {
		while (estados_iguais(*e,novo)) {
			chooseCell(*e, random);
			sscanf(random, "%d %d", &lin, &col);
			if (novo.grelha[lin][col] != VAZIA) novo.grelha[lin][col] = VAZIA;
		}
		if (resComAjudas(novo)) {cells--; *e = novo;}
		else novo = *e;
		if (control > 1000) {control = 0; ind = geraNumero(0, *n_sols-1); *e = solutions[ind]; novo = *e; cells = backupCells;}
		else control++; 
	}
	changeToFixo(e);
}

void changeToFixov2 (ESTADO *e, int x, int y) {
	if (e->grelha[x][y] == SOL_X) e->grelha[x][y] = FIXO_X;
	else e->grelha[x][y] = FIXO_O;
}

void alteraEstado (ESTADO *e, ESTADO *copy, char *random) {
	int lin, col;
	sscanf(random, "%d %d", &lin, &col);
	changeToFixov2(copy, lin, col);
	e->grelha[lin][col] = copy->grelha[lin][col];
}

void geraRandom (ESTADO *e, ESTADO *solutions, int n_sols) {
	int i = 0;
	int ind;
	if (n_sols) ind = geraNumero(0, (n_sols-1));
	else ind = 0;
	int cells = (e->num_lins * e->num_cols);
	int solvePer = geraNumero(1,2) * 0.1 * cells;
	ESTADO vazio = *e;
	ESTADO full = solutions[ind];
	char random[10];

	while (i < solvePer) {
		chooseCell(*e,random);
		alteraEstado(&vazio, &full, random);
		i++;
	}

	*e = vazio;
}

void geraDificil (ESTADO *e, ESTADO *solutions) {
	int n_sols = 0;
	int numberSols = 0;

	createState(e);
	resolveRecAll(e, solutions, &n_sols);
	geraRandom(e, solutions, n_sols);

	resolveRecAll(e, solutions, &numberSols);

	while (numberSols != 1) {
		createState(e);
		geraRandom(e, solutions, n_sols);
		numberSols = 0;
		resolveRecAll(e, solutions, &numberSols);
	}
}

void gera (ESTADO *e, ESTADO *solutions, int dif) {
	int n_sols = 0;
	createState(e);
	resolveRecAll(e, solutions, &n_sols);
	if (dif - 1) geraDificil(e, solutions);
	else geraFacil(e, solutions, &n_sols);
}