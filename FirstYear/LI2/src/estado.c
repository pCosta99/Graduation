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
#define TAM             1000 / e.num_lins
#define TAM_B           300
#define IMAGE_PATH		"http://localhost/images/"

void chooseKeep(ESTADO e) {
	int X, Y;
	for (X = 0; X < e.num_cols; X++)
		for (Y = 0; Y < e.num_lins; Y++) {
			if (e.grelha [Y][X] == VAZIA) 
				IMAGEM(X,Y,TAM,"vazio.png");
			if (e.grelha [Y][X] == BLOQUEADA)
				IMAGEM(X,Y,TAM,"bloq.png");
			if (e.grelha [Y][X] == FIXO_X)
				IMAGEM(X,Y,TAM,"X.png");
			if (e.grelha [Y][X] == FIXO_O)
				IMAGEM(X,Y,TAM,"O.png");
	}
}

void printEstado (ESTADO e) {
	char link[MAX_BUFFER];
	int X;
	int Y;

	COMECAR_HTML;
	readMacro("/var/www/html/css/background.css");
	printf("\n");

	if (e.skip) {
		invalidFile(); 
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?user=%s", e.user);
		ABRIR_SVG(2000,1000);
		ABRIR_LINK(link);
		IMAGEM2(850,200,TAM_B*2,TAM_B*2,"start.png");
		FECHAR_LINK;
		FECHAR_SVG;
		FECHAR_HTML; 
		return;
	}

	if (e.type == 1) ABRIR_SVG((TAM*e.num_cols)+800,1000);
	else if (e.type == 2 || e.gerar) ABRIR_SVG(2000,1000);

	if (e.gerar) chooseKeep(e);
	else if (!e.type) choose_user();
	else if (e.type == 2) inicializar(e);
	else if (check_win(e) && !e.solved) win(e);

	else {
		ABRIR_SVG(TAM*e.num_cols,TAM*e.num_lins);
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
				if (!e.gerar) FECHAR_LINK;
			}
		FECHAR_SVG;
	}
	if (!check_win(e) && !e.gerar) {
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?user=%s", e.user);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+500,0,TAM_B,TAM_B,"start.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,help", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+500,250,TAM_B,TAM_B,"help.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,undo", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+500,500,TAM_B,TAM_B,"undo.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,save", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+500,750,TAM_B,TAM_B,"save.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,loadA", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,0,TAM_B,TAM_B,"load.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,solve", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,250,TAM_B,TAM_B,"solve.png");
		FECHAR_LINK;

		ABRIR_LINK("http://localhost/cgi-bin/GandaGalo");
		IMAGEM2((TAM*e.num_cols)+100,500,TAM_B,TAM_B,"login.png");
		FECHAR_LINK;
	}

	else if (e.gerar) {
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?keep");
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,0,TAM_B+200,TAM_B,"keep.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?user=%s", e.user);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,350,TAM_B+200,TAM_B,"discard.png");
		FECHAR_LINK;

		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s", getenv("QUERY_STRING"));
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,700,TAM_B+200,TAM_B,"reroll.png");
		FECHAR_LINK;
	}

	if (e.type == 1 && !check_win(e) && !e.gerar) {
		sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,reset", e.filename);
		ABRIR_LINK(link);
		IMAGEM2((TAM*e.num_cols)+100,750,TAM_B,TAM_B,"reset.png");
		FECHAR_LINK;
	}

	if (e.type || e.gerar) FECHAR_SVG;
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