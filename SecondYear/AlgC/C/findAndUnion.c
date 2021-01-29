#include "findAndUnion.h"

// Subconjuntos para o algoritmo de Kruskal

// Subconjunto => Array em que cada índice corresponde a um vértice

// Inicia o subconjunto
// T(N) = Ɵ(N)
void initA (int c[]) {
	int i;

	for (i = 0; i < NW; i++) 
		c[i] = i;
}

// Indica o subconjunto de um vértice 
// T(N) = Ɵ(1)
int findA (int c[], int v) {
	return (c[v]);
}

// Une dois subconjuntos
// T(N) = Ɵ(N)
int unionA (int c[], int id1, int id2) {
	int i;

	for (i = 0; i < NW; i++)
		if (c[i] == id2)
			c[i] = id1;

	return id1;
}

// Subconjunto => Floresta de vértices

// Inicia o subconjunto
// T(N) = Ɵ(N)
void initF (int c[]) {
	int i;

	for (i = 0; i < NW; i++)
		c[i] = -1;
}

// Indica o subconjunto de um vértice
// T(N) = O(N)
int findF (int c[], int v) {
	int r;

	if (c[v] < 0) r = v;
	else {
		r = findF (c, c[v]);
		c[v] = r;
	}

	return r;
}

// Une dois subconjuntos
// T(N) = Ɵ(1) 
int unionF (int c[], int o, int d) {
	int p, f, id1, id2;

	id1 = findF(c, o);
	id2 = findF(c, d);

	if (c[id1] < c[id2]) {
		// id1 tem mais elementos
		p = id1; f = id2;
	} else {
		// id2 tem mais elementos
		p = id2; f = id1;
	}
	c[p] += c[f];
	c[f] = p;

	return p; 
}

void pushRight (EDGE e[], int start, int used) {
	int i = used-1;

	while (i != start) {
		e[i] = e[i-1];
		i--;
	}
}

void insert (EDGE e[], EDGE new, int used) {
	int i;

	for (i = 0; i < used; i++) {
		if (new.cost < e[i].cost && e[i].cost == INFINITY) {
			e[i] = new;
			break;
		}
		else if (new.cost < e[i].cost) {
			pushRight (e, i, used);
			e[i] = new;
			break;
		}
	}
}
