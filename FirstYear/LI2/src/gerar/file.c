#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "estado.h"
#include "file.h"
#include "list.h"

#define MAX_BUFFER		10240
#define TAM             40
#define USERPATH        "/var/www/html/users"
#define ESTADOPATH      "/var/www/html/ficheiros"
#define PATH            "/home/osboxes/Lab/2018li2g021/src"
#define READUSER        "/var/www/html/"

int estadotofile (ESTADO e) {
	int i, j, k;
	char fullpath[MAX_BUFFER];
	char hist[MAX_BUFFER];
	char acessTo[MAX_BUFFER];
	char readFrom[MAX_BUFFER];
	char writeIn[MAX_BUFFER];
	FILE *f;

	chmod(READUSER, S_IRWXU | S_IRWXG | S_IRWXO);
	if (e.type == 2) write_user_to_file(e); 

	sprintf(fullpath, "%s/%s/%s.e", USERPATH, e.user, e.filename);
	f = fopen(fullpath, "w");

	if (f == NULL && e.type != 1) {
		sprintf(hist, "%s/%s/hist%s.e", USERPATH, e.user, e.filename);
		sprintf(acessTo, "%s/%s", USERPATH, e.user);
		mkdir (acessTo, 777);
		chmod (acessTo, S_IRWXU | S_IRWXG | S_IRWXO);
		for (k = 1; k < 5; k++) {
			sprintf(readFrom, "%s/estadot%d.e", ESTADOPATH, k);
			sprintf(writeIn, "%s/estadot%d.e", acessTo, k);
			copy_estados(readFrom, writeIn);
			}
		if (e.type != 2) f = fopen(hist, "w");
		if (e.type != 2) fclose(f);
	}

	if (f != NULL) {
		fprintf(f, "%d %d %d %d\n", e.num_lins, e.num_cols, e.type, e.inicializado);
		for (i = 0; i < e.num_lins; i++) {
			for (j = 0; j < e.num_cols; j++) {
				if (e.grelha[i][j] == VAZIA) fprintf(f,".");      
				if (e.grelha[i][j] == FIXO_X) fprintf(f,"X");
				if (e.grelha[i][j] == FIXO_O) fprintf(f,"O");
				if (e.grelha[i][j] == BLOQUEADA) fprintf(f,"#");
				if (e.grelha[i][j] == SOL_O) fprintf(f,"o");
				if (e.grelha[i][j] == SOL_X) fprintf(f,"x");
				if (e.grelha[i][j] == INV_O) fprintf(f,"0");
				if (e.grelha[i][j] == INV_X) fprintf(f,"1");
			}
		fprintf(f, "\n");	
		}

		histToFile(e);
		fclose(f);
		free(e.action);
		free(e.filename);
	}
	else {
		free(e.action);
		free(e.filename);
	}

	return 1;
}

ESTADO get_estado_from_file (ESTADO e) {
	char fullpath[MAX_BUFFER];
	char buffer[20];
	FILE *file;

	sprintf(fullpath, "%s/%s/%s.e", USERPATH, e.user, e.filename);

	file = fopen (fullpath, "r");
	int j = 0; //marca o indice em que estamos
	int linha = 0; //marca a linha em que estamos
	/*int type;
	int lin, col;
	int inicializado;*/
	char c;

	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			j = 0;
			linha++;
		}
		else if (linha == 0) {
			buffer[j] = c;
			j++;
		}
		else if (linha > 0) {
			if (c == '.') {
				e.grelha[linha-1][j] = VAZIA;
				j++;
			}
			if (c == 'X') {
				e.grelha[linha-1][j] = FIXO_X;
				j++;
			}
			if (c == 'O') {
				e.grelha[linha-1][j] = FIXO_O;
				j++;
			}
			if (c == '#') {
				e.grelha[linha-1][j] = BLOQUEADA;
				j++;
			}
			if (c == 'x') {
				e.grelha[linha-1][j] = SOL_X;
				j++;
			}
			if (c == 'o') {
				e.grelha[linha-1][j] = SOL_O;
				j++;
			}
			if (c == '0') {
				e.grelha[linha-1][j] = INV_O;
				j++;
			}
			if (c == '1') {
				e.grelha[linha-1][j] = INV_X;
				j++;
			}
		}	
	}
	if (strlen(buffer) < 6) sscanf(buffer, "%d %d", &e.num_lins, &e.num_cols);
	else sscanf(buffer, "%d %d %d %d", &e.num_lins, &e.num_cols, &e.type, &e.inicializado);
	fclose(file);
	return e;
}

ESTADO get_user_from_file (ESTADO e) {
	char username[MAX_BUFFER];
	char fullpath[MAX_BUFFER];
	FILE *fp;
	char c;
	int i = 0;

	sprintf(fullpath, "%s/user.e", READUSER);
	fp = fopen(fullpath, "r");

	while ((c = fgetc(fp)) != EOF) {
			if (c != '\n') {
				username[i] = c;
				i++;
			}
	}

	username[i] = '\0';

	fclose(fp);

	e.user = username;

	return e;
}

void write_user_to_file (ESTADO e) {
	char fullpath[MAX_BUFFER];
	FILE *fp;
	sprintf(fullpath, "%s/user.e", READUSER);
	fp = fopen(fullpath, "w");

	fprintf(fp, "%s", e.user);

	fclose(fp);
}

void copy_estados(char *source, char *destiny) {
	FILE *fptr1, *fptr2;
    char c;

    fptr1 = fopen(source, "r");

    fptr2 = fopen(destiny, "w");
 
    c = fgetc(fptr1);

    while (c != EOF)
    {
        fprintf(fptr2, "%c", c);
        c = fgetc(fptr1);
    }
 
    fclose(fptr1);
    fclose(fptr2);
}