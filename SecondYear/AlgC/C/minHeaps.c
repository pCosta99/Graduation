#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minHeaps.h"
#include "gui.h"

//Algoritmos sobre Min-Heaps (filas com prioridade)

// Troca dois elementos em dados indíces num array
// T(N) = Ɵ(1)
void swap (ELEM v[], int i, int j) {
	int tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

// "Puxa" um elemento para a raíz da heap
// (enquanto este for menor que os outros)
// T(N) = O(log(2)N)
void bubbleUp (HEAP *h, int i) {
	int p;
	p = (i-1)/2;
	while (i > 0 && h->values[i] < h->values[p]) {
		swap(h->values,i,p);
		i = p;
		p = (i-1)/2;
	}
}

// "Empurra" um elemento para o nível mais profundo da heap 
// (enquanto este for maior que os outros)
// T(N) = O(log(2)N)
void bubbleDown (HEAP *h, int i) {
	int f = 2*i + 1;

	while (f < h->used) {
		if (f+1 < h->used && h->values[f+1] < h->values[f]) {
			//o descendente da direita e menor
			f += 1;
		}
		if (h->values[f] > h->values[i]) break;
		swap(h->values, i, f);
		i = f; f = i*2 + 1;
	}
}

// Transforma um array de inteiros numa min-heap
// T(N) = O(N*log(2)N)
HEAP * heapifyUp (ELEM *v, int tam) {
	HEAP *h = newHeap(tam*10);
	h->used = tam;
	h->values = v;

	for (int i = 1; i < tam; i++) 
		bubbleUp (h, i);

	return h;
}


// Transforma um array de inteiros numa min-heap
// T(N) = O(N)
HEAP * heapifyDown (ELEM *v, int tam) {
	HEAP *h = newHeap(tam*10);
	h->used = tam;
	h->values = v;

	for(int i = (tam-2)/2; i >= 0; i--) 
		bubbleDown(h, i);

	return h;
}

// Imprime o array representativo
// T(N) = Ɵ(N)
void print (HEAP h) {
	for (int i = 0; i < h.used; i++) 
		printf("%d ", h.values[i]);
	printf("\n");
}

// Insere um elemento numa heap.
// T(N) = O(log(2)N) (O custo é igual ao do bubbleUp)
int insertHeap (HEAP *h, ELEM x) {
	int r = 0;

	if (h->used < h->size) {
		h->values[h->used] = x;
		bubbleUp(h, h->used);
		h->used++;
		r = 1;
	}

	return r;
}

// Extraí o menor elemento da heap
// T(N) = O(log(2)N) (O custo é igual ao do bubbleDown)
int extractMin (HEAP *h, ELEM *x) {
	int r = 0;

	if (h->used != 0) {
		*x = h->values[0];
		swap(h->values, 0, (h->used - 1));
		h->used--;
		bubbleDown(h, 0);
		r = 1;
	} 

	return r;
}

// Inicializa uma heap com um dado tamanho
// T(N) = Ɵ(1)
HEAP * newHeap (int size) {
	HEAP *h = malloc(sizeof(HEAP));

	h->size = size;
	h->used = 0;
	h->values = malloc(sizeof(ELEM) * size);

	return h;
}

// Ficha 3 => Parte 2 => Ex 2.b)
// Verifica a validade de uma min-heap
// T(N) = Ɵ(N)
int minHeapOK (HEAP h) {
	int i = 0;
	int r = 1;

	for (i = 0; LEFTQ(i) < (h.used - 1); i++)
		if (h.values[LEFTQ(i)] < h.values[i] || 
			(RIGHTQ(i) < h.used && h.values[RIGHTQ(i)] < h.values[i])) {
				r = 0;
				break;
		}

	return r;
}

// Imprime a minHeap sob a forma de um array
// T(N) = Ɵ(N)
void printMinHeapArray (HEAP h) {
	for (int i = 0; i < h.used; i++) 
		printf("%d ",h.values[i]);
	puts("");
}

// Imprime a minHeap sob a forma de uma árvore
// T(N) = Ɵ(N)
void printMinHeapTree (HEAP h) {
	int print_pos[h.used];
	int i, j, k, pos, x=1, level=0;

	print_pos[0] = 0;
	for(i=0,j=1; i<h.used; i++,j++) {
		pos = print_pos[((i-1)/2)] + (i%2?-1:1)*(LINE_WIDTH/(pow(2,level+1))+1);

		for (k=0; k<pos-x;k++)
			printf("%c",i==0||i%2?' ':'-');
		printf("%d",h.values[i]);

		print_pos[i] = x = pos+1;
		if(j==pow(2,level)) {
			printf("\n");
			level++;
			x = 1;
			j = 0;
		}
	}

	puts("");
}

int GUIminHeaps (void) {
	HEAP *h = newHeap(1000);
	int printType = 1;
	int extracted;
	int value;
	int option = 9999;
	puts("--------------------------------------------------------------------------------");
	
	while (option) {
		option = mainCyclePriorityQ();
		switch (option) {
			case 1: 
				puts("Indique o valor a inserir:");
				scanf("%d", &value); 
				insertHeap(h, value);
				puts("Estado atual da árvore:");
				if (printType) printMinHeapArray(*h);
				else printMinHeapTree(*h);
				puts("--------------------------------------------------------------------------------");
				break;

			case 2: 
				extractMin(h, &extracted);
				printf("O valor removido foi %d.\n", extracted);
				puts("Estado atual da árvore:");
				if (printType) printMinHeapArray(*h);
				else printMinHeapTree(*h);
				puts("--------------------------------------------------------------------------------");
				break;

			case 3:
				puts("--------------------------------------------------------------------------------");
				parseManPriorityQ(*h, printType);
				break;

			case 4: 
				puts("--------------------------------------------------------------------------------");
				printType = parsePrintPriorityQ();

			default: 
				puts("--------------------------------------------------------------------------------");
				break;
		}
		if (option == 1) getchar();
	}

	return 0;
}
