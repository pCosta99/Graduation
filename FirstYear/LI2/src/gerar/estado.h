#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O tamanho máximo da grelha */
#define MAX_GRID		21
#define MAX_BUFFER		10240

typedef enum {BLOQUEADA, FIXO_X, FIXO_O, VAZIA, SOL_X, SOL_O, INV_X, INV_O} VALOR;

typedef struct list{
int jogada;
int ancora;
int x;
int y;
int init;
char play;
struct list *prox;
} *LISTA;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	int num_lins;
	int num_cols;
	char grelha[MAX_GRID][MAX_GRID];
	int type;
	char *action;
	char *user;
	char *filename;
	int inicializado;
	int win;
	int start;
	int control;
	int solved;
	LISTA hist;
} ESTADO;

ESTADO calc_novo_estado(ESTADO estado);

void printEstado(ESTADO estado); 

ESTADO reset(ESTADO e);

#endif
