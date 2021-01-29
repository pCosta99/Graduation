#ifndef __FATURACAO__
#define __FATURACAO__

#include "sale.h"
#include "config.h"

 /* Opaque data type defined in this module */
typedef struct faturacao *Faturacao;

/* Initializes the faturacao */
Faturacao init_faturacao();

/* Destroys the faturacao and free's the memory it retained */
void destroy_faturacao(Faturacao f);

/* Adds a product with no sales to Faturacao */
void addEmptyProductToFac(Faturacao f, char* product);

/* Bills a product in the corresponding month */
void billProduct(Faturacao f, Venda v);

/* Returns 0 if the product was sold, 1 otherwise */
int wasSold(Faturacao f, char* product);

/* Returns 0 if the product was sold, 1 otherwise */
int wasSoldInBranch(Faturacao f, char* product, int branch);

/*Returns the amount sold of a product on a certain type, branch and month*/
int getProductMonthlySalesInBranch(Faturacao f, char* product, char type, int branch, int month);

/*Returns the profit of a product on a certain type, branch and month*/
float getProductMonthlyProfitInBranch(Faturacao f, char* product, char type, int branch, int month);

/* Returns the total amount sold in a month */
long getMonthlySales(Faturacao f, int month);

/* Returns the total profitted in a month */
float getMonthlyProfit(Faturacao f, int month);

/* Returns a string containing the info about the product asked */
GPtrArray* productInfoAsString(Faturacao f, char* product);

#endif
