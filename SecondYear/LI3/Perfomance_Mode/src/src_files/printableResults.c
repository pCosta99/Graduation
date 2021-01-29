#include <stdlib.h>
#include "printableResults.h"

struct queryStruct{
    void** ptr; /* Pointer to the struct that answers the query */
    GPtrArray* (*toString)(void*); /* Turns the above struct into a array of strings */
    void (*freeFunc)(void*); /* Function supplied to free the pointers stored */
    int doesItNeedToBeConverted; /* Tells us if we need to convert the ptr into an array of strings */
    /* 0 = yes; 1 = no */
    int size;
};

PrintableResults initMultiplePrintableResults(void** ptr, GPtrArray* (*toString)(void*), void (*f)(void*), int flag, int size){
    int i;
    PrintableResults pr = malloc(sizeof(struct queryStruct));
    pr->ptr = malloc(sizeof(void*) * size);

    for(i = 0; i < size; i++){
        pr->ptr[i] = ptr[i];
    }
    pr->toString = toString;
    pr->freeFunc = f;
    pr->doesItNeedToBeConverted = flag;
    pr->size = size;

    return pr;
}

PrintableResults initPrintableResults(void* ptr, GPtrArray* (*toString)(void*), void (*f)(void*), int flag){
    return initMultiplePrintableResults(&ptr, toString, f, flag, 1);
}

/* Use only when pr->size = 1 */
GPtrArray* toArrayOfStrings(PrintableResults pr){
    GPtrArray* ret;

    if(pr->doesItNeedToBeConverted) ret = (GPtrArray*) pr->ptr[0];
    else ret = pr->toString(pr->ptr[0]);

    return ret;
}

GPtrArray** toMultipleArrayOfStrings(PrintableResults pr){
    GPtrArray** ret = malloc(sizeof(GPtrArray*) * pr->size);
    int i;

    for(i = 0; i < pr->size; i++){
        if(pr->doesItNeedToBeConverted) ret[i] = (GPtrArray*) pr->ptr[i];
        else ret[i] = pr->toString(pr->ptr[i]);
    }

    return ret;
}

void destroy_pr(PrintableResults pr){
    int i;
    for(i = 0; i < pr->size; i++){
        if(pr->freeFunc) pr->freeFunc(pr->ptr[i]);
    }
    free(pr->ptr);
    free(pr);
}
