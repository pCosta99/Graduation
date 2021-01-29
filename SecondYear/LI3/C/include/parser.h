#ifndef __PARSER__
#define __PARSER__

#include "interface.h"

/* Indicates if a number is valid inside a range */
#define VALID_NUMBER(I, B, C) !(I >= B && I <= C)

/* Indicates if a product is valid */
int valid_product(char* product);

/* Indicates if a customer is valid */
int valid_customer(char* customer);

/* Loads existing data from path to a Catalog. Returns 0 on sucess */
/* Type: 0 == CatProds, 1 == CatClientes */
int loadCatalog(SGV sgv, char* path, int type);

/* Loads sales */
void loadVendas(SGV sgv, char* path, int size_line);

#endif
