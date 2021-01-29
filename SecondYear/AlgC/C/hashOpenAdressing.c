#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashOpenAdressing.h"

// Função de hash
// Ɵ(1)
int hashOA (KeyType key) {
	int result = 0;

	if (key[0] > 64 && key[0] < 91)
		result = key[0] + 32;
	else result = key[0];

	return (result % HASHSIZE);
}

// Inicializa a tabela de hash
// T(N) = Ɵ(N)
void initTable (HashTableOA h) {
	int i;
	for (i = 0; i < HASHSIZE; i++)
		h[i].info = EMPTY;
}

// Limpa a tabela de hash
// T(N) = Ɵ(N)
void clearTable (HashTableOA h) {
	int i;
	for (i = 0; i < HASHSIZE; i++)
		h[i].info = EMPTY;
}

// Quadratic Probing

// Descobre a posição na qual um elemento pode ser inserido
// Melhor caso: T(N) = Ɵ(1) 
// Pior caso: T(N) = Ɵ(N) 
int findProbeQP (HashTableOA h, KeyType k) {
	int p = hashOA(k), tmp;
	int count, d = 1;

	for (count = HASHSIZE; count > 0 && h[p].info != EMPTY && h[p].info != DELETED && strcmp(h[p].key, k); count--) {
		p = (p+d)%HASHSIZE; d += 1;
	}

	if (strcmp(h[p].key, k)) {
		if (count == 0)
			p = -1;
		else {
			tmp = p;
			while (count > 0 && h[p].info != EMPTY && strcmp(h[p].key,k)) {
				count --;
				p = (p+d)%HASHSIZE;
				d += 1;
			}
			if (strcmp(h[p].key,k)) p = tmp;
		}
	}

	return p;
}

// Linear Probing

// Descobre a posição na qual um elemento pode ser inserido
// Melhor caso: T(N) = Ɵ(1) 
// Pior caso: T(N) = Ɵ(N) 
int findProbeLP (HashTableOA h, KeyType k) {
	int p = hashOA(k), tmp;
	int count;

	for (count = HASHSIZE; count > 0 && h[p].info != EMPTY && h[p].info != DELETED && strcmp(h[p].key, k); count--) 
		p = (p+1)%HASHSIZE;

	if (strcmp(h[p].key, k)) {
		if (count == 0)
			p = -1;
		else {
			tmp = p;
			while (count > 0 && h[p].info != EMPTY && strcmp(h[p].key,k)) {
				count --;
				p = (p+1)%HASHSIZE;
			}
			if (strcmp(h[p].key,k)) p = tmp;
		}
	}

	return p;
}

// Insere um elemento numa tabela de hash
// T(N) depende unicamente da findProbeLP
void insertTable (HashTableOA h, KeyType k, Info i, int probeType) {
	int p;

	if (probeType == LP) p = findProbeLP(h, k);
	else p = findProbeQP(h, k);

	if (p != -1) {
		if (!strcmp(h[p].key,k)) h[p].info = i;
		else {
		strcpy(h[p].key,k);
		h[p].info = i;
		}
	}
}

// Remove um elemento duma tabela de hash
// T(N) depende unicamente da findProbeLP
void deleteTable (HashTableOA h, KeyType k, int probeType) {
	int p;

	if (probeType == LP) p = findProbeLP(h, k);
	else p = findProbeQP(h, k);

	if (p != -1 && h[p].info != EMPTY && h[p].info != DELETED && !strcmp(h[p].key,k)) 
		h[p].info = DELETED;
}

int lookupTable (HashTableOA h, KeyType k, int probeType) {
	int p;

	if (probeType == LP) p = findProbeLP(h, k);
	else p = findProbeQP(h, k);

	if (p != -1 && h[p].info != EMPTY && h[p].info != DELETED && !strcmp(h[p].key,k))
		return p;

	return -1;
}
