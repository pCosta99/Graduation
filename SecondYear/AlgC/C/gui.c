#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

int mainProgram (void) {
	int option;

	puts("Estruturas de dados disponíveis: \n");
	puts("1. Árvores balanceadas (AVL Trees)");
	puts("2. Filas com prioridades (Min-Heaps)");
	puts("3. Dicionários (Hash Tables)");
	puts("0. Sair");
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

int mainCycleAVL (void) {
	int option;

	puts("Opções: \n");
	puts("1. Inserir elemento.");
	puts("2. Remover elemento.");
	puts("3. Manual das funções disponíveis.");
	puts("4. Alterar modo de impressão.");
	puts("0. Sair.");
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

int manAVL (void) {
	int option;

	puts("Opções: \n");
	puts("1. AVL maisProfundo(AVL a)");
	puts("   -> Retorna um elemento da árvore que se encontra à profundidade máxima.");
	puts("2. int alturaAVL(AVL a)");
	puts("   -> Retorna a altura da árvore.");
	puts("0. Sair.");	
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

void parseManAVL (AVL *a) {
	int option = manAVL();
	AVL b = NULL;
	int result;

	switch (option) {
		case 1: 
			b = maisProfundo(*a);
			if (b) printf("Valor obtido: %d\n", b->value);
			else puts("A árvore está vazia.");
			puts("--------------------------------------------------------------------------------");
			break;

		case 2: 
			result = alturaAVL(*a);
			printf("A altura da árvore é %d.\n", result);
			puts("--------------------------------------------------------------------------------");
			break;

		default: 
			puts("--------------------------------------------------------------------------------");
			break; 
	}
}

int printAVLTree (void) {
	int option;

	puts("Opções: \n");
	puts("1. preOrder  => (Root, Left, Right)");
	puts("2. postOrder => (Left, Right, Root)");
	puts("3. inOrder   => (Left, Root, Right)");
	puts("0. Sair.");
	scanf("%d", &option);

	return option;
}

int parsePrintAVLTree (void) {
	int option = printAVLTree();
	int printType;

	if (option == 1) printType = 2;
	else if (option == 2) printType = 1;
	else if (option == 0) printType = -1;
	else printType = 0;

	return printType;
}

int mainCyclePriorityQ (void) {
	int option;

	puts("Opções: \n");
	puts("1. Inserir elemento.");
	puts("2. Remover elemento.");
	puts("3. Manual das funções disponíveis.");
	puts("4. Alterar modo de impressão.");
	puts("0. Sair.");
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

int manPriorityQ (void) {
	int option;

	puts("Opções: \n");
	puts("1. int minHeapOK(HEAP h)");
	puts("   -> Indica a validade de uma min-heap.");
	puts("2. HEAP * heapifyDown(ELEM *v)");
	puts("   -> Transforma um array de ELEM (inteiros) numa min-heap, \n atráves de várias chamadas de bubbleDown.");
	puts("3. HEAP * heapifyUp(ELEM *v)");
	puts("   -> Transforma um array de ELEM (inteiros) numa min-heap, \n atráves de várias chamadas de bubbleUp.");
	puts("0. Sair.");	
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

int * readArray (int tam) {
	int element;
	int * arr = malloc(sizeof(ELEM)*tam);

	puts("Insira os elementos do array:");
	for (int i = 0; i < tam; i++) {
		scanf("%d", &element);
		arr[i] = element;
	}

	puts("--------------------------------------------------------------------------------");

	return arr;
}

void parseManPriorityQ (HEAP h, int printType) {
	int option = manPriorityQ();
	int tam;
	int *arr;
	HEAP *r;

	switch (option) {
		case 2: puts("--------------------------------------------------------------------------------");
				puts("Indique o tamanho do array:");
				scanf("%d", &tam);
				arr = readArray(tam);
				r = heapifyDown(arr, tam);
				if (printType) printMinHeapArray(*r);
				else printMinHeapTree(*r);
				break;

		case 3: puts("--------------------------------------------------------------------------------");
				puts("Indique o tamanho do array:");
				scanf("%d", &tam);
				arr = readArray(tam);
				r = heapifyUp(arr, tam);
				if (printType) printMinHeapArray(*r);
				else printMinHeapTree(*r);
				break;

		case 1: if (minHeapOK(h)) puts("A minHeap inserida é válida.");
				else puts ("A minHeap inserida não é válida");

		default:
		        puts("--------------------------------------------------------------------------------");
				break;
	}
}

int printPriorityQ (void) {
	int option;

	puts("Opções: \n");
	puts("1. Sob a forma de um array.");
	puts("2. Sob a forma de uma árvore.");
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);

	return option;
}

int parsePrintPriorityQ (void) {
	int option = printPriorityQ();
	int printType;

	if (option == 1) printType = 1;
	else printType = 0;

	return printType;
}

int mainCycleHashTable (void) {
	int option;

	puts("Opções: \n");
	puts("1. Inserir elemento.");
	puts("2. Remover elemento.");
	puts("3. Procurar elemento.");
	puts("4. Manual das funções disponíveis.");
	puts("5. Imprimir a tabela.");
	puts("0. Sair.");
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

int manHashTables (void) {
	int option;

	puts("Opções: \n");
	puts("1. int repetidos(int v[], int N)");
	puts("   -> Indica se existem elementos repetidos num array.");
	puts("2. int count(HashTable h, int i)");
	puts("   -> Determina quantas das chaves armazenadas na tabela têm hash i.");
	puts("3. int last(HashTable h, int i)");
	puts("   -> Determina a posição da última chave com hash i.");
	puts("4. int lastP(HashTable h, int i)");
	puts("   -> Determina a posição do último elemento que poderia ter sido inserido na posição i se estivesse livre.");
	puts("0. Sair.");	
	puts("Insira o número da opção desejada:");
	scanf("%d", &option);
	getchar();

	return option;
}

void parseManHashTables (HashTable h) {
	int option = manHashTables();
	int *arr;
	int r, tam, hash;

	switch (option) {
		case 1: puts("--------------------------------------------------------------------------------");
				puts("Indique o tamanho do array:");
				scanf("%d", &tam);
				arr = readArray(tam);
				r = repetidos(arr, tam);
				if (r) puts("Existem elementos repetidos no array.");
				else puts("Não existem elementos repetidos no array.");
				break;

		case 2: puts("--------------------------------------------------------------------------------");
				puts("Insira a hash que quer utilizar.");
				scanf("%d", &hash);
				r = count(h, hash);
				printf("Existem %d chaves armazenadas na tabela com hash %d.\n", r, hash);
				break;

		case 3: puts("--------------------------------------------------------------------------------");
				puts("Insira a hash que quer utilizar.");
				scanf("%d", &hash);
				r = last(h, hash);
				if (r != -1) printf("A última chave com hash %d encontra-se na posição %d.\n", hash, r);
				else printf("Não existe nenhuma chave com hash %d.\n", hash);
				break;

		case 4: puts("--------------------------------------------------------------------------------");
				puts("Insira a hash que quer utilizar.");
				scanf("%d", &hash);
				r = last(h, hash);
				if (r != -1) printf("A posição do último elemento que poderia ter sido inserido na posicão %d caso esta estivesse livre corresponde a %d.\n", hash, r);
				else printf("Não existe nenhuma chave com hash %d.\n", hash);
				break;

		default:puts("--------------------------------------------------------------------------------");
				break;
	}
}

void parsePrintHashTable (HashTable h) {
	int l_bound;
	int u_bound;
	int i, trigger = 0, member = 4;

	puts("Insira entre que indíces quer imprimir a tabela.");
	puts("Limite inferior:");
	scanf("%d", &l_bound);
	puts("Limite superior:");
	scanf("%d", &u_bound);

	puts("--------------------------------------------------------------------------------");

	i = l_bound;
	while (member) {
		if (i == u_bound+1) {
			member--;
			i = l_bound;
			if (!trigger) printf("\n");
			trigger = 0;
		}
		else {
			if (h[i].probeC >= 0) {
				switch (member) {
					case 4: if (i != u_bound) printf("Índice: %d || ", i);
							else {printf("Índice: %d\n", i); trigger = 1;}
							break;
					case 3: if (i != u_bound) printf("Chave: %d || ", h[i].key);
							else {printf("Chave: %d\n", h[i].key); trigger = 1;}
							break;
					case 2: if (i != u_bound) printf("Valor: %d || ", h[i].value);
							else {printf("Valor: %d\n", h[i].value); trigger = 1;}
							break;
					case 1: if (i != u_bound) printf("Probe: %d || ", h[i].probeC);
							else {printf("Probe: %d\n", h[i].probeC); trigger = 1;}
							break;
				}
			}
			i++;
		}
	}
}
