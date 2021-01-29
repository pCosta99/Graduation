#include <stdio.h>
#include <stdlib.h>
#include "avlTree.h"
#include "gui.h"

// Algoritmos sobre árvores balanceadas AVL 

// Tree printing

// (Root, Left, Right)
void preOrder (AVL a) {

	if (a == NULL) 
		return ;

	printf("%d ", a->value);

	preOrder(a->left);

	preOrder(a->right);
}

// (Left, Right, Root)
void postOrder (AVL a) {
	if (a == NULL)
		return ;

	postOrder(a->left);

	postOrder(a->right);

	printf("%d ", a->value);
}

// (Left, Root, Right)
void inOrder (AVL a) {
	if (a == NULL) 
		return ;

	inOrder(a->left);

	printf("%d ", a->value);

	inOrder(a->right);
}

// Auxiliar da fromList
AVL fromListRec (LInt * l, int n) {
	AVL a;
	if (n <= 0)
		return NULL;
	
	a = malloc(sizeof(struct avl));
	a->left = fromListRec(l, n/2);
	a->value = (*l)->value;
	(*l) = (*l)->prox;
	if (a->left == NULL) a->bal = BAL;
	else a->bal = LEFT;
	a->right = fromListRec(l, n-n/2-1);
	if (a->right) {
		if (a->right->bal != a->left->bal)
			a->bal = LEFT;
		else a->bal = BAL;
	}
	
	return a;
}

// Sebenta Estruturas de Dados ex.44
// T(N) = Ɵ(N)
AVL fromList (LInt l, int n) {
	return (fromListRec(&l, n));
}

// Ficha 3 -> Parte 4 (AVL) -> Ex. 1
// T(N) = Ɵ(log(2)N)
int alturaAVL (AVL a) {
	int height = 0;

	if (a == NULL) ;
	else {
		while (a->right || a ->left) {
			if (a->bal == LEFT) a = a->left;
			else a = a->right;
			height++;
		}
		height++;
	}

	return height;
}

// Pergunta 2 a) Teste 2017
// T(N) = Ɵ(log(2)N)
AVL maisProfundo (AVL a) {
	if (a == NULL) return NULL;
	while (a->left || a->right) {
		if (a->bal == LEFT) a = a->left;
		else a = a->right;
	}
	return a;
}

// Roda uma árvore para a direita
// T(N) = Ɵ(1)
AVL rotateRight (AVL a) {
	AVL b = a->left;
	a->left = b->right;
	b->right = a;
	return b;
}

// Roda uma árvore para a esquerda
// T(N) = Ɵ(1)
AVL rotateLeft (AVL b) {
	AVL a = b->right;
	b->right = a->left;
	a->left = b;
	return a; 
}

// Corrige o diferencial de alturas na sub-árvore direita
// T(N) = Ɵ(1)
AVL fixRight (AVL a) {
	AVL b, c;
	b = a->right;
	if (b->bal == RIGHT) {
		a->bal = b->bal = BAL;
		a = rotateLeft(a);
	} else {
		c = b->left;
		switch (c->bal) {
			case LEFT:  a->bal = BAL;
					    b->bal = RIGHT;
					    break;
		    case RIGHT: a->bal = LEFT;
		    			b->bal = BAL;
		    			break;
		    case BAL:   a->bal = b->bal = BAL;
		}
		c->bal = BAL;
		a->right = rotateRight(b);
		a = rotateLeft(a);
	}
	return a;
}

// Corrige o diferencial de alturas na sub-árvore esquerda
// T(N) = Ɵ(1)
AVL fixLeft (AVL a) {
	AVL b, c;
	b = a->left;
	if (b->bal == LEFT) {
		a->bal = b->bal = BAL;
		a = rotateRight(a);
	} else {
		c = b->right;
		switch (c->bal) {
			case LEFT:  a->bal = RIGHT;
					    b->bal = BAL;
					    break;
		    case RIGHT: a->bal = BAL;
		    			b->bal = LEFT;
		    			break;
		    case BAL:   a->bal = b->bal = BAL;
		}
		c->bal = BAL;
		a->left = rotateLeft (b);
		a = rotateRight(a);
	}
	return a;
}

// T(N) = O(log(2)N)
AVL updateAVLRec (AVL a, int val, int *g, int *u) {
	if (a == NULL) {
		a = malloc(sizeof(struct avl));
		a->value = val;
		a->bal = BAL;
		a->left = a->right = NULL;
		*g = 1; *u = 0;
	}
	else if (a->value > val) {
		a->left = updateAVLRec(a->left, val, g, u);
		if (*g == 1)
			switch (a->bal) {
				case LEFT:  a = fixLeft(a); *g = 0; break;
				case BAL:   a->bal = LEFT; break;
				case RIGHT: a->bal = BAL; *g = 0; break;
			}
	} else  {
		a->right = updateAVLRec(a->right, val, g, u);
		if (*g == 1)
			switch (a->bal) {
				case RIGHT: a = fixRight(a); *g = 0; break;
				case BAL:   a->bal = RIGHT; break;
				case LEFT:  a->bal = BAL; *g = 0; break;
			}
	}
	return a;
}

// Insere um elemento numa árvore
int updateAVL (AVL *a, int val) {
	int g, u;
	*a = updateAVLRec(*a, val, &g, &u);
	return u;
}

// Remove um elemento de uma árvore
// T(N) = O(log(2)N)
AVL removeFromAVL (AVL *a, int key, int *g) {
	AVL *current = a;
	AVL node;

	if (*a == NULL) {
		node = NULL;
		*g = 0;
	}
	else if ((*a)->value == key) {
		node = *a;
		if ((*a)->right) *a = (*a)->right;
		else if ((*a)->left) *a = (*a)->left;
		else *a = NULL;
		*g = 1;
	}
	else if (key > (*a)->value) {
		a = &((*a)->right);
		node = removeFromAVL(a, key, g);
		if (*g == 1) {
			switch ((*current)->bal) {
				case LEFT:  *current = rotateRight(*current); break;
				case BAL:   (*current)->bal = LEFT; break;
				case RIGHT: (*current)->bal = BAL; break;
			}
		}
	} else {
		a = &((*a)->left);
		node = removeFromAVL(a, key, g);
		if (*g == 1) {
			switch((*current)->bal) {
				case RIGHT: *current = rotateLeft(*current); break;
				case BAL:   (*current)->bal = RIGHT; break;
				case LEFT:  (*current)->bal = BAL; break;
			}
		}
	}

	return node;
}

// Retorna o elemento removido
AVL removeAVL (AVL *a, int key) {
	AVL node;
	int g;

	node = removeFromAVL(a, key, &g);
	node->left = NULL;
	node->right = NULL;

	return node;
}

int GUIavlTree (void) {
	AVL a = NULL;
	int printType = 1;
	int value;
	int option = 9999;
	puts("--------------------------------------------------------------------------------");
	
	while (option) {
		option = mainCycleAVL();
		switch (option) {
			case 1: 
				puts("Indique o valor a inserir:");
				scanf("%d", &value); 
				updateAVL(&a, value);
				puts("Estado atual da árvore:");
				if (printType == 2) preOrder(a);
				else if (printType == 1) postOrder(a);
				else inOrder(a);
				puts("\n--------------------------------------------------------------------------------");
				break;

			case 2: 
				puts("Indique o valor a remover:");
				scanf("%d",&value);
				removeAVL(&a, value);
				puts("Estado atual da árvore:");
				if (printType == 2) preOrder(a);
				else if (printType == 1) postOrder(a);
				else inOrder(a);
				puts("\n--------------------------------------------------------------------------------");
				break;

			case 3:
				puts("--------------------------------------------------------------------------------");
				parseManAVL(&a);
				break;

			case 4:
				puts("--------------------------------------------------------------------------------");
				printType = parsePrintAVLTree();
				if (printType != -1) {
					puts("Estado atual da árvore:");
					if (printType == 2) preOrder(a);
					else if (printType == 1) postOrder(a);
					else inOrder(a);
					puts("");
				}

			default: 
				puts("--------------------------------------------------------------------------------");
				break;
		}
		if(option == 1 || option == 2) getchar();
	}

	return 0;
}
