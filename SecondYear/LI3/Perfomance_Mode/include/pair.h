#ifndef __PAIR__
#define __PAIR__

/*
 * Simple API that defines a pair
 */

typedef struct pair *Pair;

Pair mkPair(void* x, void* y);

void freePair(void* p);

void* fst(Pair p);

void* snd(Pair p);

#endif
