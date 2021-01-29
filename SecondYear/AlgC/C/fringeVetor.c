#include <stdio.h>
#include <stdlib.h>
#include "fringeVetor.h"

// T(N) = Ɵ(N)
FRINGE newFringe(int tam) {
	FRINGE f;

	f.cost = malloc(sizeof(int)*tam);
	f.tam = tam;

	for (int i = 0; i < tam; i++) {
		f.cost[i] = -1;
	}
	
	return f;
}

// T(N) = Ɵ(1)
void addV(FRINGE f, int v, int peso) {
	f.cost[v] = peso;
}

// T(N) = Ɵ(N)
FRINGE nextF(FRINGE f, int *v) {
	int i, min, minIndex;

	min = INFINITY; minIndex = f.tam-1;

	for (i = 0; i < f.tam; i++) 
		if (f.cost[i] != -1 && f.cost[i] < min) {
			minIndex = i;
			min = f.cost[i];
		}

	f.cost[minIndex] = -1;
	*v = minIndex;

	return f;
}

// T(N) = Ɵ(1)
FRINGE updateV (FRINGE f, int v, int peso) {
	if (peso < f.cost[v])
		f.cost[v] = peso;

	return f;
}
