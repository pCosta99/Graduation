#include "config.h"
#include "catalog.h"
#include "catclientes.h"

struct catclientes{
    Catalog customers;
    int validCustomers, totalCustomers;
};

CatClientes initCatClientes(){
    CatClientes c = malloc(sizeof(struct catclientes));
    c->customers = init_catalog(26, 1, 4000, 1000, free);
    return c;
}

void destroyCatClientes(CatClientes c){
    destroy_catalog(c->customers);
    free(c);
}

void insertCustomer(CatClientes c, char* customer){
    catalog_insert(c->customers, customer, customer);
}

int customerExists(CatClientes c, char* customer){
    return catalog_exists(c->customers, customer);
}

GPtrArray* allCustomers(CatClientes c){
    GPtrArray* array = g_ptr_array_new_with_free_func(free);

    catalog_foreach(c->customers, (GTraverseFunc) insert_in_array, array);

    return array;
}

int getTotalCustomers(CatClientes c){
    return c->totalCustomers;
}

void setTotalCustomers(CatClientes c, int totalCustomers){
    c->totalCustomers = totalCustomers;
}

int getValidCustomers(CatClientes c){
    return c->validCustomers;
}

void setValidCustomers(CatClientes c, int validCustomers){
    c->validCustomers = validCustomers;
}
