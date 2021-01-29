#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "sale.h"

struct sale{
    char* product;
    float price;
    int amount;
    char type;
    char* customer;
    int month;
    int branch;
};

/* Construtor parametrizado */
Venda createSale(char* product, float price, int amount, char type, char* customer, int month, int branch){
    Venda v = malloc(sizeof(struct sale));

    v->product = my_strdup(product);
    v->price = price;
    v->amount = amount;
    v->type = type;
    v->customer = my_strdup(customer);
    v->month = month;
    v->branch = branch;

    return v;
}

char* getProduct(Venda v){
    return my_strdup(v->product);
}

float getPrice(Venda v){
    return v->price;
}

int getAmount(Venda v){
    return v->amount;
}

char getType(Venda v){
    return v->type;
}

char* getCustomer(Venda v){
    return my_strdup(v->customer);
}

int getMonth(Venda v){
    return v->month;
}

int getBranch(Venda v){
    return v->branch;
}

void setProduct(Venda v, char* product){
    free(v->product);
    v->product = my_strdup(product);
}

void setPrice(Venda v, float price){
    v->price = price;
}

void setAmount(Venda v, int amount){
   v->amount = amount;
}

void setType(Venda v, char type){
    v->type = type;
}

void setCustomer(Venda v, char* customer){
    v->customer = customer;
}

void setMonth(Venda v, int month){
    v->month = month;
}

void setBranch(Venda v, int branch){
    v->branch = branch;
}

void destroy_venda(Venda v){
    free(v->product);
    free(v->customer);
    free(v);
}

/*
int main(void){
    Venda v = createSale("AB1234", 72.34, 100, 'N', "Z1234", 10, 2);

    char* p = getProduct(v);

    setProduct(v, p);

    free(p);
    destroy_venda(v);

    return 0;
}*/
