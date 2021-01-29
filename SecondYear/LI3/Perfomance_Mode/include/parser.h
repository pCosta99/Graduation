#ifndef __PARSER__
#define __PARSER__

#include "sgv.h"

/* Loads existing data from path to a Catalog. Returns 0 on sucess */
/* Type: 0 == CatProds, 1 == CatClientes */
int loadCatalog(SGV sgv, char* path, int size_line, int type);

/* Loads sales */
void loadVendas(SGV sgv, char* path, int size_line);

#endif
