#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashChaining.h"

// Ficha 3 => Parte 3 => Tabelas de Hash (Chaining ou Closed Adressing) 

// Função de hash
// Ɵ(1)
int hashChain (KeyType key) {
	int result = 0;

	if (key[0] > 64 && key[0] < 91)
		result = key[0] + 32;
	else result = key[0];

	return (result % HASHSIZE_C);
}

// Inicializa a tabela de hash
// T(N) = Ɵ(N)
void initChain (HashTableC h) {
	int i;
	for (i = 0; i < HASHSIZE_C; i++) 
		h[i] = NULL;
}

// Limpa a tabela de hash
// T(N) = Ɵ(N)
void clearChain (HashTableC h) {
	int i;
	for (i = 0; i < HASHSIZE_C; i++) {
		h[i] = NULL;
	}
}

// Insere um elemento numa tabela de hash
// Melhor caso: T(N) = Ɵ(1)
// Pior caso: T(N) = Ɵ(N)
void insertChain (HashTableC h, KeyType k, Info i) {
	int index = hashChain(k);
	EntryC *pt;

	for (pt = h[index]; pt != NULL && strcmp(pt->key, k); pt = pt->next);

	if (pt != NULL) pt->info = i;
	else {
		pt = malloc (sizeof(struct entryc));
		strcpy(pt->key, k);
		pt->info = i;
		pt->next = h[index];
		h[index] = pt;
	}

}

// Remove um elemento duma tabela de hash
// T(N) = Ɵ(1) (Not sure)
void deleteChain (HashTableC h, KeyType k) {
	int index = hashChain(k);
	struct entryc *temp = h[index], *prev;

	if (temp != NULL && !strcmp(temp->key, k)) {
		h[index] = temp->next;
		free(temp);
		return;
	}

	while (temp != NULL && strcmp(temp->key, k)) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) return;

	prev->next = temp->next;
	free(temp);
}

// Procura um elemento numa tabela de hash
// Melhor caso: T(N) = Ɵ(1)
// Pior caso: T(N) = Ɵ(N)
EntryC * lookupChain (HashTableC h, KeyType k) {
	int index = hashChain(k);
	EntryC *pt;

	for (pt = h[index]; pt != NULL && strcmp(pt->key, k); pt = pt->next);

	return pt;
}
