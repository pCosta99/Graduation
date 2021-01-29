#ifndef __FILIAL__
#define __FILIAL__

#include "sale.h"
#include "config.h"

typedef struct filial *Filial;

Filial init_filial();

void destroy_filial(Filial f);

void addEmptyCustomerToBranch(Filial f, char* customer);

void customerBought(Filial f, Venda v);

/* Returns 0 if the customer bought at least 1 product. 1 otherwise */
int didCustomerBought(Filial f, char* customer);

/* Finds the products bought by a client */
void getProductsBoughtByClientInBranch(Filial f, int* monthlyAmounts, char* customer);

/* Returns the lists of buyers of a product (divided by type)  */
GPtrArray** getProductBuyersInBranch(Filial f, char* product);

/* Fills the given hastable with products as keys and their bought amounts as values */
void getClientFavProducts(Filial f, char* customer, int month, GHashTable* table);

/* Fills the given hashtable with the number of customers and the number of sales of the products of this branch */
void getSoldProductsInBranch(Filial f, GHashTable* table, int* numAndCurrentBranch);

/* Fills the hashtable provided with the amount spent in each product by a customer */
void getClientTopProfitProductsBranch(Filial f, char* clientID, GHashTable *table);

#endif
