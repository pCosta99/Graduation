#include <stdlib.h>
#include "pair.h"

struct pair{
    void* x;
    void* y;
};

Pair mkPair(void* x, void* y){
    Pair p = malloc(sizeof(struct pair));

    p->x = x;
    p->y = y;

    return p;
}

void freePair(void* ptr){
    Pair p = (Pair) ptr;
    free(p);
}

void* fst(Pair p){
    return p->x;
}

void* snd(Pair p){
    return p->y;
}
