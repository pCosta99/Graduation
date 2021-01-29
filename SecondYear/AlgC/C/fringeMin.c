#include <stdlib.h>
#include "fringeMin.h"

void swapV (PAIR v[], int i, int j) {
	PAIR tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

FRINGEmin bubbleDown (FRINGEmin f, int i) {
	int s = 2*i + 1;

	while (s < f.used) {
		if (s+1 < f.used && (f.values[s+1]).cost < (f.values[s]).cost) {
			//o descendente da direita e menor
			s += 1;
		}
		if ((f.values[s]).cost > (f.values[i]).cost) break;
		swapV(f.values, i, s);
		i = s; s = i*2 + 1;
	}

	return f;
}

FRINGEmin bubbleUp (FRINGEmin f, int i) {
	int p;
	p = (i-1)/2;
	while (i > 0 && (f.values[i]).cost < (f.values[p]).cost) {
		swapV(f.values,i,p);
		i = p;
		p = (i-1)/2;
	}
	return f;
}

// T(N) = Ɵ(N)
FRINGEmin newFringeMin(int tam) {
	FRINGEmin f;

	f.size = tam;
	f.used = 0;
	f.values = malloc(sizeof(PAIR)*tam);

	return f;
}

// T(N) = O(log(2)N)
FRINGEmin addMin(FRINGEmin f, int v, int peso) {
	PAIR p;

	p.v = v;
	p.cost = peso;

	if (f.used < f.size) {
		f.values[f.used] = p;
		f = bubbleUp(f, f.used);
		f.used++;
	}

	return f;
}

// T(N) = O(log(2)N)
FRINGEmin nextMin(FRINGEmin f, int *v) {
	PAIR p;

	if (f.used != 0) {
		p = f.values[0];
		*v = p.v;
		swapV(f.values, 0, (f.used-1));
		f.used--;
		f = bubbleDown(f, 0);
	}

	return f;
}

// T(N) = O(log(2)N)
FRINGEmin updateMin(FRINGEmin f, int v, int peso) {
	int i;

	for (i = 0; (f.values[i]).v != v; i++) ;

	(f.values[i]).cost = peso;

	f = bubbleUp(f, i);

	return f;
}
