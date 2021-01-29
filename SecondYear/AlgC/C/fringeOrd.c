#include <stdlib.h>
#include "fringeOrd.h"

void swap (int v[], int i, int j) {
	int tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

// T(N) = Ɵ(N)
FRINGEord newFringeOrd(int tam) {
	FRINGEord f;

	f.inicio = f.fim = 0;
	f.vertices = malloc(sizeof(int) * tam);

	return f;
}

// T(N) = O(N)
FRINGEord addOrd(FRINGEord f, int v, int costV[]) {
	int i, j;

	for (i = f.inicio; i != f.fim && costV[v] > costV[f.vertices[i]]; i++);

	if (i != f.fim) {
		for (j = f.fim; j != i; j--) 
			f.vertices[j] = f.vertices[j-1]; 
		f.vertices[i] = v;
	}
	else f.vertices[f.fim] = v;

	f.fim++;

	return f;
}

// T(N) = Ɵ(1)
FRINGEord nextOrd(FRINGEord f, int *v) {
	*v = f.vertices[f.inicio++];

	return f;
}

// T(N) = Ɵ(N)
FRINGEord updateOrd(FRINGEord f, int v, int costV[]) {
	int i, j, r = f.inicio, off = 0;

	for (i = f.inicio; f.vertices[i] != v ; i++)
		if (costV[v] < costV[f.vertices[i]]) {
			off++;
			if (r == f.inicio) r = i;
		}
 
	for (j = r; j < i && off; j++, off--)
		swap (f.vertices, j, i);

	return f;
}
