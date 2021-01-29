#ifndef ___CHECK_H___
#define ___CHECK_H___

int check_grelha (ESTADO e, int X, int Y, int control, char symbol);

int check_linha(ESTADO *e, int a, int b, int control);

int check_estado(ESTADO e);

ESTADO compara_estados(ESTADO e, ESTADO novo); 

int check_inv(ESTADO e); 

int check_win(ESTADO e);

int check_full_tab (ESTADO e);

int estados_iguais (ESTADO e, ESTADO b);

ESTADO turn_inv (ESTADO e);

ESTADO turn_valid (ESTADO e);

int checkVazio (ESTADO e);

#endif