#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"

#define MAX_BUFFER		10240
#define USERPATH        "/var/www/html/users"

ESTADO push(ESTADO e, int X, int Y, char val) {
	LISTA l = e.hist;
	LISTA new = malloc(sizeof(struct list));

	new->x = X;
	new->y = Y;
	new->play = val;
	new->jogada = (l->jogada) + 1;
	new->prox = l;
	new->ancora = 0;

	e.hist = new;
	return e;
}

ESTADO pop(ESTADO e) {
	LISTA l = e.hist;
	LISTA t = l->prox;

	e.grelha[l->x][l->y] = l->play;

	e.hist = t;
	return e;
}

void histToFile(ESTADO e) {
	char fullpath[MAX_BUFFER];
	FILE *fp;

	sprintf(fullpath, "%s/%s/hist%s.e", USERPATH, e.user, e.filename);
	fp = fopen(fullpath,"w");

	LISTA l = e.hist;

	while(l != NULL) {
		fprintf(fp, "%d %d %d %d %d ", l->ancora, l->jogada, l->x, l->y, l->init);
		if (l->play == VAZIA) fprintf(fp,".");
		if (l->play == SOL_O) fprintf(fp,"o");
		if (l->play == SOL_X) fprintf(fp,"x");
		if (l->play == INV_O) fprintf(fp,"0");
		if (l->play == INV_X) fprintf(fp,"1");
		fprintf(fp, "\n");
		l = (l->prox);
	}

	fclose(fp);
}

LISTA revL (LISTA l) {
	LISTA r, t;
	r = NULL;
	while (l != NULL) {
		t = l; l = l -> prox;
		t -> prox = r; r = t;
	}
	return r;
}

ESTADO fileToHist(ESTADO e) {
	char *fullpath;
	char buffer[15];
	fullpath = malloc(sizeof(char)*1024);
	FILE *fp;
	LISTA current, head;
	head = current = NULL;

	sprintf(fullpath, "%s/%s/hist%s.e", USERPATH, e.user, e.filename);
	fp = fopen(fullpath, "r");

	if (fp == NULL) return e;

	while(fgets(buffer, 15, fp)) {
		LISTA node = malloc(sizeof(struct list));
		sscanf(buffer, "%d %d %d %d %d %c", &node->ancora, &node->jogada, &node->x, &node->y, &node->init, &node->play);
		if (node->play == '.') node->play = VAZIA;
		else if (node->play == 'o') node->play = SOL_O;
		else if (node->play == 'x') node->play = SOL_X;
		else if (node->play == '0') node->play = INV_O;
		else if (node->play == '1') node->play = INV_X;
		node -> prox = head;
		head = node;
	}

	e.hist = revL(head);
	fclose(fp);

	return e;
}