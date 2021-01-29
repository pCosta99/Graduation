#ifndef MIN_HEAPS
#define MIN_HEAPS

#define LINE_WIDTH 70

#define PARENTQ(i) (i-1)/2 // o índice do array começa em 0
#define LEFTQ(i)   2*i + 1
#define RIGHTQ(i)  2*i + 2

typedef int ELEM; //elementos da heap

typedef struct heap {
	int size;
	int used;
	ELEM *values;
} HEAP;

void swap (ELEM v[], int i, int j);

void bubbleUp (HEAP *h, int i);

void bubbleDown (HEAP *h, int i);

HEAP * heapifyUp (ELEM *v, int tam);

HEAP * heapifyDown (ELEM *v, int tam);

void print (HEAP h);

int insertHeap (HEAP *h, ELEM x);

int extractMin (HEAP *h, ELEM *x);

HEAP * newHeap (int size);

int minHeapOK (HEAP h);

void printMinHeapArray (HEAP h);

void printMinHeapTree (HEAP h);

int GUIminHeaps (void);

#endif
