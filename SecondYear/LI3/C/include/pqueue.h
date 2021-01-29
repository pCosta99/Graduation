#ifndef __PQUEUE__H__
#define __PQUEUE__H__

/**
* Debugging macro .
*
* Checks for a NULL pointer, and prints the error message, source file and
* line via 'stderr' .
* If the check fails the program exits with error code (-1) .
*/
#define NP_CHECK(ptr)                                                           \
    {                                                                           \
        if (NULL == (ptr)) {                                                    \
            fprintf(stderr, "%s:%d NULL POINTER: %s n",                         \
                __FILE__, __LINE__, #ptr);                                      \
            exit(-1);                                                           \
        }                                                                       \
    }                                                                           \

#define DEBUG(msg) fprintf(stderr, "%s:%d %s", __FILE__, __LINE__, (msg))

typedef struct key_value *KeyValue;

/**
* Priority Queue Structure
*/
typedef struct PQueue_s {
    /* The actual size of heap at a certain time */
    size_t size;
    /* The amount of allocated memory for the heap */
    size_t capacity;
    /* An array of (void*), the actual max-heap */
    KeyValue *data;
    /* A pointer to a comparator function, used to prioritize elements */
    int (*cmp)(const void *d1, const void *d2);
    /* A pointer to a free function, used to free keys */
    void (*free_key)(void* key);
    /* A pointer to a free function, used to free values */
    void (*free_value)(void* value);
} PQueue;

/** Allocates memory for a new Priority Queue .
Needs a pointer to a comparator function, thus establishing priorities .
*/
PQueue *pqueue_new(int (*cmp)(const void *d1, const void *d2),
                   void (*free_key)(void* ptr),
                   void (*free_value)(void* value),
                   size_t capacity);

/** De-allocates memory for a given Priority Queue */
void pqueue_delete(void *q);

/** Add an element inside the Priority Queue */
void pqueue_enqueue(PQueue *q, const void *key, const void *value);

/** Removes the element with the greatest priority from within the Queue */
/** Retrieves the element value */
void *pqueue_dequeue(PQueue *q);

#endif
