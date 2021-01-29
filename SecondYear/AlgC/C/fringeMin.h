#ifndef FRINGE_MIN
#define FRINGE_MIN

typedef struct pair {
	int v;
	int cost;
} PAIR;

typedef struct fringemin {
	int size;
	int used;
	PAIR *values;
} FRINGEmin;

void swapV (PAIR v[], int i, int j);

FRINGEmin bubbleDown (FRINGEmin f, int i);

FRINGEmin bubbleUp (FRINGEmin f, int i);

FRINGEmin newFringeMin(int tam);

FRINGEmin addMin(FRINGEmin f, int v, int peso);

FRINGEmin nextMin(FRINGEmin f, int *v);

FRINGEmin updateMin(FRINGEmin f, int v, int peso);

#endif
