#ifndef ___RESOLVE_ESTADO_H___
#define ___RESOLVE_ESTADO_H___

ESTADO invert (ESTADO e);

ESTADO rotate (ESTADO e);

void colocaXO (ESTADO *e, int putX);

void solveVazioRet (ESTADO e, ESTADO *solutions, int *n_sols);

void solveVazioQuadrado (ESTADO e, ESTADO *solutions, int *n_sols);

void resolveRecAll (ESTADO *e, ESTADO *solutions, int *n_sols);

#endif