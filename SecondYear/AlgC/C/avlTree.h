#ifndef AVL_TREE
#define AVL_TREE

#define LEFT   1
#define BAL    0
#define RIGHT -1

typedef struct llint {
	int value;
	struct llint *prox;
} *LInt;

typedef struct avl {
	int value;
	int bal;
	struct avl *left, *right;
} *AVL;

void preOrder(AVL a);

void postOrder(AVL a);

void inOrder(AVL a);

AVL fromListRec(LInt * l, int n);

AVL fromList(LInt l, int n);

int alturaAVL(AVL a);

AVL maisProfundo(AVL a);

AVL rotateRight(AVL a);

AVL rotateLeft(AVL a);

AVL fixRight(AVL a);

AVL fixLeft(AVL a);

AVL updateAVLRec (AVL a, int val, int *g, int *u);

int updateAVL (AVL *a, int val);

AVL removeFromAVL (AVL *a, int key, int *g);

AVL removeAVL (AVL *a, int key);

int GUIavlTree (void);

#endif
