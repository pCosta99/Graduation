#ifndef __CUSTOMER_CATALOG__
#define __CUSTOMER_CATALOG__

#include "config.h"

typedef struct catclientes *CatClientes;

/*Inits the customer catalog*/
CatClientes initCatClientes();

/*Frees the customer catalog*/
void destroyCatClientes(CatClientes c);

/*Inserts a customer in the customer catalog*/
void insertCustomer(CatClientes c, char* customer);

/*Checks if a customer exists in the catalog*/
int customerExists(CatClientes c, char* customer);

/*Returns all the customers that the catalog contains*/
GPtrArray* allCustomers(CatClientes c);

int getTotalCustomers(CatClientes c);

void setTotalCustomers(CatClientes c, int totalCustomers);

int getValidCustomers(CatClientes c);

void setValidCustomers(CatClientes c, int validCustomers);

#endif
