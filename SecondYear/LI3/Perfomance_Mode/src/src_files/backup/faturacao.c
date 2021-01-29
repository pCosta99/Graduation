#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "sale.h"
#include "faturacao.h"

#define NUM_BRANCHES         3
#define NUM_LETTERS          26
#define PRODUCTS_DEPTH       2
#define NUM_MESES            12

#define CUSTOMERS_DEPTH      1
#define CUSTOMERS_RANGE      4000
#define CUSTOMER_LOWER_LIMIT 1000

struct productInfo{
    /* Everything that gets splitted into 2 differs in Normal vs Promotion */
    char* product;
    int totalSales;
    float total_profit[2];
    int* monthly_sales[2];
    float* monthly_profit[2];
    GTree* buyers;
};

typedef struct productInfo *ProductInfo;

int customerCodeToInt(char* code){
    int letterValue = code[0] - 65;
    int numValue = atoi(code+CUSTOMERS_DEPTH);

    return ((letterValue * CUSTOMERS_RANGE) + (numValue - CUSTOMER_LOWER_LIMIT));
}

/* Function to compare customer codes with the same type of return values of strcmp */
int compare_chars(const void* gpa, const void* gpb, void* something){
    char* a = (char*) gpa;
    char* b = (char*) gpb;

    int a_int = customerCodeToInt(a);
    int b_int = customerCodeToInt(b);

    return (a_int - b_int);
}

ProductInfo createProductInfo(char* product){
    int i, j;
    ProductInfo pi = malloc(sizeof(struct productInfo));

    pi->product = my_strdup(product);
    pi->totalSales = 0;
    pi->buyers = g_tree_new_full(compare_chars, NULL, free, free);
    for(i = 0; i < 2; i++){
        pi->total_profit[i] = 0;
        pi->monthly_sales[i] = malloc(sizeof(int) * NUM_MESES);
        pi->monthly_profit[i] = malloc(sizeof(float) * NUM_MESES);
        for(j = 0; j < NUM_MESES; j++){
            pi->monthly_sales[i][j] = 0;
            pi->monthly_profit[i][j] = 0;
        }
    }

    return pi;
}

void destroy_productInfo(void* ptr){
    ProductInfo pi = (ProductInfo) ptr;
    int i;
    free(pi->product);
    for(i = 0; i < 2; i++){
        free(pi->monthly_sales[i]);
        free(pi->monthly_profit[i]);
    }
    g_tree_destroy(pi->buyers);
    free(pi);
}

void accumulateSaleOnInfo(ProductInfo pi, Venda v){
    float profit = getPrice(v) * getAmount(v);
    int month = getMonth(v);
    char* customer = getCustomer(v);
    int* n_buys = (int*) g_tree_lookup(pi->buyers, customer);
    int index = getType(v) == 'P';

    if(n_buys == NULL) {
        int* code = malloc(sizeof(int));
        (*code) = 1;
        g_tree_insert(pi->buyers, customer, code);
    }
    else {
        (*n_buys)++;
        free(customer);
    }

    pi->totalSales++;
    pi->total_profit[index] += profit;
    pi->monthly_sales[index][month-1]++;
    pi->monthly_profit[index][month-1] += profit;
}


struct faturacao {
    /* catalogs[0]: invalid; catalogs[1]: valid; */
    /* catalogs[0]: (key,value) -> (int, NULL); catalogs[1]: (key,value) -> (int, ProductInfo); */
    Catalog catalogs[2][NUM_BRANCHES];
};

/* Initializes the structure */
Faturacao init_faturacao(){
    int i, j;
    Faturacao f = malloc(sizeof(struct faturacao));
    for(j = 0; j < 2; j++){
        for(i = 0; i < NUM_BRANCHES; i++){
            /* The invalid catalog (j = 0) only has codes as keys. The valid catalog takes codes as keys and ProductInfo as values. */
            if(j) f->catalogs[j][i] = init_catalog(NUM_LETTERS, PRODUCTS_DEPTH, 9000, 1000, destroy_productInfo);
            else f->catalogs[j][i] = init_catalog(NUM_LETTERS, PRODUCTS_DEPTH, 9000, 1000, free);
        }
    }
    return f;
}

/* Destroys the faturacao and free's the memory it retained */
void destroy_faturacao(Faturacao f){
    int i, j;
    for(j = 0; j < 2; j++){
        for(i = 0; i < NUM_BRANCHES; i++){
           destroy_catalog(f->catalogs[j][i]);
        }
    }
    free(f);
}

/* Returns a catalog */
Catalog getCatalog(Faturacao f, int branch, int sold){
    return f->catalogs[sold][branch-1];
}

/* Bills a product */
void billProduct(Faturacao f, Venda v){
    char* product = getProduct(v);
    int branch = getBranch(v);

    Catalog c = f->catalogs[1][branch-1];
    ProductInfo pi = (ProductInfo) catalog_get(c, product);
    if(pi){
        accumulateSaleOnInfo(pi, v);
    } else {
        pi = createProductInfo(product);
        accumulateSaleOnInfo(pi, v);
        catalog_insert(c, product, pi);
    }

    free(product);
}

/*
int main(void){
    Faturacao f = init_faturacao();

    Venda v = createSale("AA1234", 10.00, 100, 'N', "A1234", 3, 2);
    billProduct(f, v);
    billProduct(f, v);

    destroy_venda(v);
    destroy_faturacao(f);

    return 0;
}*/
