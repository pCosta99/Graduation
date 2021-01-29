#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "estado.h"
#include "ler_estado.h"
#include "check.h"
#include "ajuda.h"
#include "list.h"
#include "resolveEstado.h"
#include "file.h"

#define MAX_BUFFER		10240
#define TAM             40

void printEstado (ESTADO e) {
	char link[MAX_BUFFER];
	int X;
	int Y;

	COMECAR_HTML;

	if (e.type) ABRIR_SVG(2000,2000);

	if (!e.type) choose_user();
	else if (e.type == 2) inicializar();
	else if (check_win(e) && !e.solved) win(e);

	else {
		for (X = 0; X < e.num_cols; X++)
			for (Y = 0; Y < e.num_lins; Y++) {
				sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,s%d_%d", e.filename, Y, X);
				ABRIR_LINK(link);

				if (e.grelha [Y][X] == VAZIA) 
					IMAGEM(X,Y,TAM,"vazio.png");
				if (e.grelha [Y][X] == BLOQUEADA)
					IMAGEM(X,Y,TAM,"bloq.png");
				if (e.grelha [Y][X] == SOL_X)
					IMAGEM(X,Y,TAM,"X.png");
				if (e.grelha [Y][X] == FIXO_X)
					IMAGEM(X,Y,TAM,"X.png");
				if (e.grelha [Y][X] == SOL_O)
					IMAGEM(X,Y,TAM,"O.png");
				if (e.grelha [Y][X] == FIXO_O)
					IMAGEM(X,Y,TAM,"O.png");
				if (e.grelha [Y][X] == INV_O)
					IMAGEM(X,Y,TAM,"IO.png");
				if (e.grelha [Y][X] == INV_X)
					IMAGEM(X,Y,TAM,"IX.png");
				FECHAR_LINK;
			}
	}
	if (!check_win(e)) {
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?t,%s,user=%s", e.filename, e.user);
		ABRIR_LINK(link);
		IMAGEM(4,2,150,"novo.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,help", e.filename);
		ABRIR_LINK(link);
		IMAGEM(6,2,150,"help.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,undo", e.filename);
		ABRIR_LINK(link);
		IMAGEM(6,0,150,"undo.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,save", e.filename);
		ABRIR_LINK(link);
		IMAGEM(4,0,150,"save.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,loadA", e.filename);
		ABRIR_LINK(link);
		IMAGEM(4,1,150,"load.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,solve", e.filename);
		ABRIR_LINK(link);
		IMAGEM(4,3,150,"solve.png");
		FECHAR_LINK;
	}

	if (e.type == 1 && !check_win(e)) {
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,reset", e.filename);
		ABRIR_LINK(link);
		IMAGEM(6,1,150,"reset.png");
		FECHAR_LINK;
	}

	if (e.type) FECHAR_SVG;
	FECHAR_HTML;
}

ESTADO calc_novo_estado (ESTADO e) {
	ESTADO *solutions = malloc(sizeof(ESTADO)*50);
	ESTADO novo = e;
	int n_sols = 0;

	if (e.action == NULL) {
		return e;
	}

	else if (!strcmp(e.action, "show")) {
		e.solved = 1;
		return e;
	}

	else if (!strcmp(e.action, "solve")) {
		e = reset(e);
		resolveRecAll(&e, solutions, &n_sols);
		if (n_sols) e = *solutions;
		free(solutions);
		return get_user_from_file(e);
	}

	else if (!strcmp(e.action, "save")) {
		novo.hist->ancora = 1;
		return novo;
	}

	else if (!strcmp(e.action, "loadA")) {
		if (novo.hist->init == 0) return e;
		while (!(novo.hist->ancora) && novo.hist->init == 1) {
			novo = pop(novo);
		}

		novo.hist->ancora = 0;
		if (estados_iguais(reset(novo), novo)) return e;

		return novo;
	}

	else if (!strcmp(e.action, "undo")) {
		if (!novo.hist->init) return e;
		novo = pop(e);

		if(check_inv(novo)) return turn_valid(novo);

		return novo;
	}

	else if (!strcmp(e.action, "help")) {
		novo = ajuda(e, 1, 0);
		
		if (check_estado(novo)) {
			if (check_inv(novo)) return turn_valid(novo);
			else return novo;
		}

		return e;
	}

	else if (e.action[0] == 's') {
		int X, Y;
		sscanf(e.action, "s%d_%d", &X, &Y);

		if (e.grelha[X][Y] == VAZIA) novo.grelha[X][Y] = SOL_X;
		if (e.grelha[X][Y] == SOL_X) novo.grelha[X][Y] = SOL_O;
		if (e.grelha[X][Y] == SOL_O) novo.grelha[X][Y] = VAZIA;
		if (e.grelha[X][Y] == INV_X) novo.grelha[X][Y] = SOL_O;
		if (e.grelha[X][Y] == INV_O) novo.grelha[X][Y] = VAZIA;

		if (check_inv(novo) == 1) return turn_valid(novo);
		else if (check_inv(novo) == 0) return e;


		if (!check_estado(novo)) novo = turn_inv(novo);

		if (!estados_iguais(novo,e)) {
			novo = push(novo, X, Y, e.grelha[X][Y]);
			novo.hist->init = 1;
		}

		return novo;
	}

	else if (!strcmp(e.action, "load")) {
		e.type = 1;
		e.inicializado = 1;
		return e;
	}

	else if (!strcmp(e.action, "reset")) {
		novo = reset(e);
		return novo;
	}

	return e;
}

ESTADO reset (ESTADO e) {
	ESTADO novo = e;
	int X;
	int Y;
	for (X = 0; X < e.num_lins; X++) 
		for (Y = 0; Y < e.num_cols; Y++) {
			if (e.grelha[X][Y] == SOL_X || e.grelha[X][Y] == SOL_O || e.grelha[X][Y] == INV_X || e.grelha[X][Y] == INV_O) novo.grelha[X][Y] = VAZIA;
		}
	novo.hist = NULL;
	return novo;
}