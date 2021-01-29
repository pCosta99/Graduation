#ifndef GUI
#define GUI

#include "avlTree.h"
#include "minHeaps.h"
#include "hashProbeCounting.h"

int mainProgram(void);

int mainCycleAVL(void);

int manAVL(void);

void parseManAVL (AVL *a);

int printAVLTree (void);

int parsePrintAVLTree (void);

int mainCyclePriorityQ (void);

int manPriorityQ (void);

int * readArray (int tam);

void parseManPriorityQ (HEAP h, int printType);

int printPriorityQ (void);

int parsePrintPriorityQ (void);

int mainCycleHashTable (void);

int manHashTables (void);

void parseManHashTables (HashTable h);

void parsePrintHashTable (HashTable h);

#endif
