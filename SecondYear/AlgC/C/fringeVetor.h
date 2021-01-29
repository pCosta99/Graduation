#ifndef FRINGE_VETOR
#define FRINGE_VETOR

#define INFINITY 9999

typedef struct fringe {
	int *cost;
	int tam;
} FRINGE;

FRINGE newFringe(int tam);

void addV(FRINGE f, int v, int peso);

FRINGE nextF(FRINGE f, int *v);

FRINGE updateV(FRINGE f, int v, int peso);

#endif
