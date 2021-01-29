#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "hashProbeCounting.h"

// Inicia uma tabela de hash
// T(N) = Ɵ(N)
void init (HashTable h) {
	for (int i = 0; i < HASHSIZE_PC; i++)
		h[i].probeC = FREE;
}

// Função de hash
// Ɵ(1)
int hash (int key) {
	return (key % HASHSIZE_PC);
}

// Insere um elemento numa tabela de hash
// Melhor caso: T(N) = Ɵ(1)
// Pior caso: T(N) = Ɵ(N)
int update (HashTable h, int key, int value) {
	int p = hash(key), probe = 0;

	while (h[p].probeC >= 0 && probe < HASHSIZE_PC && h[p].key != key) {
		p = (p+1) % HASHSIZE_PC;
		probe++;
	}

	if (probe == HASHSIZE_PC) return 2; //overflow (deu a volta completa a tabela)
	if (h[p].probeC >= 0) {
		h[p].value = value;
		return 1; //existe 
	}
	h[p].key = key;
	h[p].value = value;
	h[p].probeC = probe;

	return 0; //Elemento inserido
}

// Procura uma chave numa tabela de hash
// Melhor caso: T(N) = Ɵ(1)
// Pior caso: T(N) = Ɵ(N)
int lookup (HashTable h, int key, int *value) {
	int p =  hash(key), probe = 0;

	while (h[p].probeC >= 0 && probe < HASHSIZE_PC && h[p].key != key) {
		p = (p+1)%HASHSIZE_PC;
		probe++;
	}

	if (probe == HASHSIZE_PC) return 2; //overflow (deu a volta completa a tabela)
	if (h[p].key == key) {
		*value = h[p].value;
		return 1; //existe
	}

	return 0; //não existe
}

// Função auxiliar da removeKey que trata de puxar os elementos para os novos lugares vazios
int compress (HashTable h, int free) {
	int p = (free+1)%HASHSIZE_PC, off = 1, trigger = 0;

	while (h[p].probeC >= 0) {
		if (h[p].probeC >= off) {
			// Puxa um elemento para trás
			h[free] = h[p];
			h[free].probeC = h[p].probeC - off;
			// Marca o lugar atual como vazio
			h[p].probeC = FREE;
			off = 0;
			free = p;
			trigger = 1;
		}
		p = (p+1)%HASHSIZE_PC;
		off++;
	}

	return trigger;
}

// Remove um elemento duma tabela de hash, ocupando o novo espaço criado
// O tempo desta função depende da quantidade de trocas que vão ser feitas
int removeKey (HashTable h, int key) {
	int p = hash(key), probe = 0;

	while (h[p].probeC >= 0 && probe < HASHSIZE_PC) {
		if (h[p].key == key) {
			if (!compress(h, p)) h[p].probeC = -1;
			return 1;
		}
		p = (p+1)%HASHSIZE_PC;
		probe++;
	}

	return 0;
}

// Teste 2018 => Ex.2)
// Retorna 1 se há elementos repetidos no array
// A tabela de hash deve ter, no mínimo, o dobro do tamanho do array inserido (2*N).
// T(N) = Ɵ(N)
int repetidos (int v[], int N) {
	HashTable h;
	int i, r = 0;

	init(h);

	for (i = 0; i < N && r != 1; i++) 
		r = update(h, v[i], v[i]);

	return r;
}

// Sebenta => Estruturas de Dados => p.49 ex.21)
int count (HashTable h, int i) {
	int counter = 0;
	int r = i;

	while (h[r].probeC >= 0) {
		if (h[r].probeC == r-i)
			counter++;
		r++;
	} 

	return counter;
}

// Sebenta => Estruturas de Dados => p.49 ex.22)
int last (HashTable h, int i) {
	int counter = -1;
	int trigger = 0;
	int r = i;

	while (h[r].probeC >= 0) {
		if (h[r].probeC == r-i) {
			counter++;
			trigger++;
		}
		else if (!trigger) counter++; 
		r++;
	}

	if (!trigger) r = -1;
	else r = i + counter;
	
	return r;
}

// Sebenta => Estruturas de Dados => p.49 ex.23)
int lastP (HashTable h, int i) {
	int r = i;
	int trigger = 1;

	while (h[r].probeC >= 0) {
		if (h[r].probeC == r-i) {
			trigger = 0;
			break;
		}
		r++;
	}

	if (trigger) r = -1;

	return r; 
}

int GUIhashTables (void) {
	HashTable h;
	int value, key, trigger;
	int option = 9999;

	init(h);
	puts("--------------------------------------------------------------------------------");
	
	while (option) {
		option = mainCycleHashTable();
		switch (option) {
			case 1: 
				puts("Indique a chave a inserir:");
				scanf("%d", &key);
				puts("Indique o valor a inserir:");
				scanf("%d", &value); 
				update(h, key, value);
				puts("--------------------------------------------------------------------------------");
				break;

			case 2: 
				puts("Indique a chave a remover:");
				scanf("%d", &key);
				trigger = removeKey(h, key);
				if (trigger) printf("A chave %d foi removida com sucesso. \n", key);
				else printf("Não foi possível encontrar a chave %d. \n", key);
				puts("--------------------------------------------------------------------------------");
				break;

			case 3:
				puts("Indique a chave a procurar:");
				scanf("%d", &key);
				trigger = lookup(h, key, &value);
				if (trigger == 1) printf("A chave %d corresponde ao valor %d. \n", key, value);
				else printf("Não foi possível encontrar a chave %d. \n", key);
				puts("--------------------------------------------------------------------------------");
				break;

			case 4:
				puts("--------------------------------------------------------------------------------");
				parseManHashTables(h);
				break;

			case 5: 
				puts("--------------------------------------------------------------------------------");
				parsePrintHashTable(h);

			default: 
				puts("--------------------------------------------------------------------------------");
				break;
		}
		if (option == 1) getchar();
	}

	return 0;
}
