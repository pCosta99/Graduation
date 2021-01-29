#ifndef ___LIST_H___
#define ___LIST_H___

ESTADO push(ESTADO e, int X, int Y, char val);

ESTADO pop(ESTADO e);

void histToFile(ESTADO e);

ESTADO fileToHist(ESTADO e);

LISTA revL (LISTA l);

#endif