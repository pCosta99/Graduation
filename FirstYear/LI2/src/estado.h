#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que alteram o estado e o imprimem
*/

/** \brief O tamanho máximo da grelha */
#define MAX_GRID		21

typedef enum {BLOQUEADA, FIXO_X, FIXO_O, VAZIA, SOL_X, SOL_O, INV_X, INV_O} VALOR;

/**
\brief Estrutura que armaneza o histórico
*/
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
	int gerar;
	int numberGen;
	int keepGera;
	int skip;
	LISTA hist;
} ESTADO;

/**
Função que imprime as peças da grelha
@param e O estado
*/
void chooseKeep(ESTADO e); 

/**
Função que imprime o UI (User Interface)
@param e O estado
*/
void printEstado(ESTADO e); 

/**
Função que altera um estado consoante a ação obtida através da QUERY_STRING
@param e O estado a ser alterado
@returns O estado alterado
*/
ESTADO calc_novo_estado(ESTADO e);

/**
Função que dá reset num estado, isto é, apaga todas as casas que não são fixas ou bloqueadas
@param e O estado atual
@returns O estado inicial
*/
ESTADO reset(ESTADO e);

#endif
