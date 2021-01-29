#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "estado.h"
#include "ler_estado.h"
#include "file.h"
#include "list.h"

#define MAX_BUFFER		10240
#define TAM             40

ESTADO ler_estado () {
	ESTADO e = {0};

	char *query_string = getenv("QUERY_STRING");

	if (query_string == NULL || strlen(query_string) == 0) {
		return e;
	}

	if (query_string[0] == 'u') {
		e.type = 2;
		e.inicializado = 1;

		e.user = malloc(sizeof(char)*1024);
		get_user(query_string, e.user);

		return e;
	}

	if (query_string[0] == 't') {
		e.type = 2;
		e.inicializado = 1;
		e.control = 1;

		query_string += 2;

		e.filename = malloc(sizeof(char)*1024);
		get_file_name(query_string, e.filename);

		query_string += 9;

		e.user = malloc(sizeof(char)*1024);
		get_user(query_string, e.user);


		return e;
	}

	if (query_string[0] == 'w') {
		e.win = 1;
		query_string += 2;
	}
	e.filename = malloc(sizeof(char)*1024);
	e.action = malloc(sizeof(char)*1024);
	e.user = malloc(sizeof(char)*1024);

	e = get_user_from_file(e);
	get_file_name(query_string , e.filename);
	if (!e.win) get_action(query_string, e.action);
	e = get_estado_from_file(e);
	e = fileToHist(e);

	if (e.hist == NULL) {
		e.hist = malloc(sizeof(struct list)); 
		e.hist->jogada = 0;
	}


	if (e.win) {
		e = reset(e);
		e.type = 2;
	}

	return e;
}

void get_user (char *query_string, char *user) {
	int i = 0;
	int j;

	while (query_string[i] != '=') {
		i++;
	}

	i++;

	for (j = 0; query_string[j] != '\0'; j++) {
		user[j] = query_string[i+j];
	}

	user[j] = '\0';
}

void get_file_name (char *query_string, char *filename) {
	int i;

	for (i = 0; query_string[i] != ',' && query_string[i] != '\0'; i++) {
		filename[i] = query_string[i];
	}

	filename[i] = '\0';
}

void get_action (char *query_string, char *action) {
	int i = 0;
	int j;

	while (query_string[i] != ',') {
		i++;
	}

	i++;

	for (j = 0; query_string[j] != ',' && query_string[j] != '\0'; j++) {
		action[j] = query_string[i+j];
	}

	action[j] = '\0';
}

/**
Função que inicializa o estado

Esta função produz um estado em que o número de linhas e de colunas é igual ao valor de GRELHA e atribuí valores a certas casas.

@returns Um novo estado
*/
void inicializar() {
	ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?estadot1,load");
	IMAGEM(0,0,350, "tab1.png");
	FECHAR_LINK;

	ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?estadot2,load");
	IMAGEM(0,1,350, "tab2.png");
	FECHAR_LINK;

	ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?estadot3,load");
	IMAGEM(2,0,350, "tab3.png");
	FECHAR_LINK;

	ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?estadot4,load");
	IMAGEM(2,1,350, "tab4.png");
	FECHAR_LINK;
}

void choose_user() {
	ABRIR_FORM("http://localhost/cgi-bin/GandaGalo");
	ABRIR_DIV;
	ABRIR_LABEL;
	SUBMIT_BOX("Username", "user", 20);
	FECHAR_LABEL;
	FECHAR_DIV;
	ABRIR_DIV;
	SUBMIT_BUTTON("submit", "That's my name!");
	FECHAR_DIV;
	FECHAR_FORM;
}

void win(ESTADO e) {
	char link[MAX_BUFFER];
	IMAGEM(1,0,600,"win.png");

	sprintf(link, "http://localhost/cgi-bin/GandaGalo?w,%s,%s", e.filename, e.user);
	ABRIR_LINK(link);
	IMAGEM(4,2,200,"playagain.png");
	FECHAR_LINK;

	sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,show", e.filename);
	printf("<a href=%s target=\"_blank\">Show solved puzzle.\n", link);
	IMAGEM(4,3,200,"show.png");
	FECHAR_LINK;
}