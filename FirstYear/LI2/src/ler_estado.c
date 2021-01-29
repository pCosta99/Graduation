#define _BSD_SOURCE
#include <unistd.h>
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

	else if (query_string[0] == 'L') {
		int map;
		query_string += 3;
		e = get_user_from_file(e);
		int nGerados = numeroGerados(e);
		e.filename = malloc(sizeof(char)*1024);
		map = atoi(query_string);
		if (map && map <= nGerados) {
			strcpy(e.filename, "r");
			strcat(e.filename, query_string);
			e = get_estado_from_file(e);
			e.type = 1;
		} 
		else {e = get_user_from_file(e); e.skip = 1;} 
	}

	else if (query_string[0] == 'k') {
		int nGerados;
		e.keepGera = 1;
		e.filename = malloc(sizeof(char)*1024);
		e.user = malloc(sizeof(char)*1024);
		e = get_user_from_file(e);
		e = get_estado_from_file(e);
		nGerados = numeroGerados(e);
		sprintf(e.filename,"r%d", nGerados+1);
		e.type = 1;
		return e;
	}

	else if (query_string[0] == 'u') {
		e.type = 2;
		e.inicializado = 1;

		e.user = malloc(sizeof(char)*1024);
		get_user(query_string, e.user);

		return e;
	}

	else if (query_string[0] == 't') {
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

	else if (query_string[0] == 'w') {
		e.win = 1;
		query_string += 2;
	}

	else if (query_string[0] == 'l') {
		FILE *p;
		int control, i, x, y, write;
		control = i = x = y = write = 0;
		char ch;
		char buffer[MAX_BUFFER];
		char buffer2[10];
		char sysCall[100];
		int lins, cols, dif;
		sscanf(query_string, "lins=%d&cols=%d&dif=%d", &lins, &cols, &dif);
		sprintf(sysCall, "/usr/lib/cgi-bin/gerar %d %d %d", dif, lins, cols);
		p = popen(sysCall, "r");
		while((ch=fgetc(p)) != EOF)
        	buffer[write++] = ch;
       	pclose(p);
        buffer[write] = '\0';
        write = 0;
		while(buffer[i]) {
			if (buffer[i] == '\n') {
				if (control) {x++; y = 0;}
				else control++;
			}
			else if (control) {
				switch (buffer[i]) { 
					case ('.') : 
						e.grelha[x][y++] = VAZIA;
						break;
					case ('X') : 
						e.grelha[x][y++] = FIXO_X;
						break;
					case ('O') : 
						e.grelha[x][y++] = FIXO_O;
						break;
					case ('#') : 
						e.grelha[x][y++] = BLOQUEADA;
						break;
				}
			}
			else {
				buffer2[write++] = buffer[i];
			}
			i++;
		}
		sscanf(buffer2, "%d %d", &e.num_lins, &e.num_cols);
		e.gerar = 1;
		e = get_user_from_file(e);
	}

	else {
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

	for (j = 0; query_string[i+j] != ',' && query_string[i+j] != '\0'; j++) {
		action[j] = query_string[i+j];
	}

	action[j] = '\0';
}

void inicializar(ESTADO e) {
	readMacro("/var/www/html/css/gerador.css");
	printf("<div class=\"relative\">");
    printf("Number of maps: %d", numeroGerados(e));
	printf("</div>");
}

void choose_user() {
	readMacro("/var/www/html/css/login.css");
}

void win(ESTADO e) {
	char link[MAX_BUFFER];
	IMAGEM(1,0,600,"win.png");

	sprintf(link, "http://localhost/cgi-bin/GandaGalo?user=%s", e.user);
	ABRIR_LINK(link);
	IMAGEM(4,2,200,"playagain.png");
	FECHAR_LINK;

	sprintf(link, "http://localhost/cgi-bin/GandaGalo?%s,show", e.filename);
	printf("<a href=%s target=\"_blank\">Show solved puzzle.\n", link);
	IMAGEM(4,3,200,"show.png");
	FECHAR_LINK;
}

void invalidFile() {
	readMacro("/var/www/html/css/invalid.css");

	printf("\n<div class=\"relative\">\n");
    printf("Could not load that map.\n");
	printf("</div>\n");
}