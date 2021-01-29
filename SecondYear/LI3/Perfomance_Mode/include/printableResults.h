#ifndef __PRINTABLE_RESULTS__
#define __PRINTABLE_RESULTS__

#include <gmodule.h>

typedef struct queryStruct *PrintableResults;

PrintableResults initMultiplePrintableResults(void** ptr, GPtrArray* (*toString)(void*), void (*f)(void*), int flag, int size);

PrintableResults initPrintableResults(void* ptr, GPtrArray* (*toString)(void*), void (*f)(void*), int flag);

/* Use only when pr->size = 1 */
GPtrArray* toArrayOfStrings(PrintableResults pr);

/* Use when pr->size >= 1 */
GPtrArray** toMultipleArrayOfStrings(PrintableResults pr);

/* Frees the memory alloc'd */
void destroy_pr(PrintableResults pr);


#endif
