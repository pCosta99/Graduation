#ifndef FRINGE_ORD
#define FRINGE_ORD

typedef struct fringeord {
	int inicio;
	int fim;
	int * vertices;
} FRINGEord;

void swap (int v[], int i, int j);

FRINGEord newFringeOrd(int tam);

FRINGEord addOrd(FRINGEord f, int v, int costV[]);

FRINGEord nextOrd(FRINGEord f, int *v);

FRINGEord updateOrd(FRINGEord f, int v, int costV[]);

#endif
