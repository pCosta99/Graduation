#include <stdlib.h>
#include <stdio.h>

#include "pqueue.h"

/* Util macros */
#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x) / 2)

void pqueue_heapify(PQueue *q, size_t idx);

struct key_value{
    void* key;
    void* value;
};

/**
* Allocates memory for a new Priority Queue structure .

* 'cmp' function:
*   returns 0 if d1 and d2 have the same priorities
*   returns [negative value] if d1 have a smaller priority than d2
*   returns [positive value] if d1 have a greater priority than d2
*/
PQueue *pqueue_new(int (*cmp)(const void *d1, const void *d2),
                   void (*free_key)(void* ptr),
                   void (*free_value)(void* value),
                   size_t capacity) {
    PQueue *res = NULL;
    NP_CHECK(cmp);
    res = malloc(sizeof(*res));
    NP_CHECK(res);
    res->cmp = cmp;
    /* The inner representation of data inside the queue is an array of KeyValue's */
    res->data = malloc(capacity * sizeof(KeyValue));
    NP_CHECK(res->data);
    res->size = 0;
    res->capacity = capacity;
    res->free_key = free_key;
    res->free_value = free_value;
    return (res);
}

/**
* De-allocates memory for a given Priority Queue structure .
*/
void pqueue_delete(void* ptr) {
    PQueue *q = (PQueue*) ptr;
    size_t i;
    if (NULL == q) {
        DEBUG("Priority Queue is already NULL. Nothing to free.");
        return;
    }
    for(i = 0; i < q->size; i++){
        if(q->data[i]){
            if(q->free_key) q->free_key(q->data[i]->key);
            if(q->free_value) q->free_value(q->data[i]->value);
            free(q->data[i]);
        }
    }
    free(q->data);
    free(q);
}

/**
* Adds a new element to the Priority Queue .
*/
void pqueue_enqueue(PQueue *q, const void *key, const void *value) {
    size_t i;
    KeyValue tmp = NULL;
    NP_CHECK(q);
    if (q->size >= q->capacity) {
        DEBUG("Priority Queue is full. Cannot add another element .");
        return;
    }
    KeyValue kv = malloc(sizeof(struct key_value));
    kv->key = (void*) key;
    kv->value = (void*) value;
    /* Adds element last */
    q->data[q->size] = kv;
    i = q->size;
    q->size++;
    /* The new element is swapped with its parent as long as its
    precedence is higher */
    while(i > 0 && q->cmp(q->data[i]->key, q->data[PARENT(i)]->key) > 0) {
        tmp = q->data[i];
        q->data[i] = q->data[PARENT(i)];
        q->data[PARENT(i)] = tmp;
        i = PARENT(i);
    }
}

/**
* Returns the value of the element with the biggest priority from the queue .
*/
void *pqueue_dequeue(PQueue *q) {
    KeyValue data = NULL;
    void *tmp_value = NULL;
    NP_CHECK(q);
    if (q->size < 1) {
         /* Priority Queue is empty */
         return NULL;
    }
    data = q->data[0];
    tmp_value = data->value;
    if(q->free_key) free(data->key);
    free(data);
    q->data[0] = q->data[q->size-1];
    q->size--;
    /* Restore heap property */
    pqueue_heapify(q, 0);
    return tmp_value;
}

/**
* Turn an "almost-heap" into a heap .
*/
void pqueue_heapify(PQueue *q, size_t idx) {
    /* left index, right index, largest */
    KeyValue tmp = NULL;
    size_t l_idx, r_idx, lrg_idx;
    NP_CHECK(q);

    l_idx = LEFT(idx);
    r_idx = RIGHT(idx);

    /* Left child exists, compare left child with its parent */
    if (l_idx < q->size && q->cmp(q->data[l_idx]->key, q->data[idx]->key) > 0) {
        lrg_idx = l_idx;
    } else {
        lrg_idx = idx;
    }

    /* Right child exists, compare right child with the largest element */
    if (r_idx < q->size && q->cmp(q->data[r_idx]->key, q->data[lrg_idx]->key) > 0) {
        lrg_idx = r_idx;
    }

    /* At this point largest element was determined */
    if (lrg_idx != idx) {
        /* Swap between the index at the largest element */
        tmp = q->data[lrg_idx];
        q->data[lrg_idx] = q->data[idx];
        q->data[idx] = tmp;
        /* Heapify again */
        pqueue_heapify(q, lrg_idx);
    }
}
