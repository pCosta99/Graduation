#ifndef __FATURACAO__
#define __FATURACAO__

#include <gmodule.h>
#include "sale.h"
#include "catalog.h"

 /* Opaque data type defined in this module */
typedef struct faturacao *Faturacao;

/* Initializes the faturacao */
Faturacao init_faturacao();

/* Destroys the faturacao and free's the memory it retained */
void destroy_faturacao(Faturacao f);

/* Bills a product */
void billProduct(Faturacao f, Venda v);

#endif
