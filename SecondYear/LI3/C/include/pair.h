#ifndef __PAIR__
#define __PAIR__

/*
 * Simple API that defines a pair
 */

typedef struct pair *Pair;

Pair mkPair(void* x, void* y);

Pair mkPairWithFreeFunc(void* x, void (*free_x)(void*), void* y, void (*free_y)(void*));

void freePair(void* p);

void* fst(Pair p);

void* snd(Pair p);

#endif
