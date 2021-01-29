#include <stdlib.h>
#include "pair.h"

struct pair{
    void* x;
    void (*free_x)(void*);

    void* y;
    void (*free_y)(void*);
};

Pair mkPair(void* x, void* y){
    Pair p = malloc(sizeof(struct pair));

    p->x = x;
    p->free_x = NULL;
    p->y = y;
    p->free_y = NULL;

    return p;
}

Pair mkPairWithFreeFunc(void* x, void (*free_x)(void*), void* y, void (*free_y)(void*)){
    Pair p = malloc(sizeof(struct pair));

    p->x = x;
    p->free_x = free_x;
    p->y = y;
    p->free_y = free_y;

    return p;
}

void freePair(void* ptr){
    Pair p = (Pair) ptr;

    if(p->free_x) p->free_x(p->x);
    if(p->free_y) p->free_y(p->y);

    free(p);
}

void* fst(Pair p){
    return p->x;
}

void* snd(Pair p){
    return p->y;
}
