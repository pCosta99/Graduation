#include "pair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "sale.h"
#include "catalog.h"
#include "faturacao.h"

#define NUM_LETTERS          26
#define PRODUCTS_DEPTH       2
#define NUM_MESES            12
#define NUM_BRANCHES         3

struct productInfo{
    char* product;
    int sales[2][NUM_BRANCHES][NUM_MESES];
    float profit[2][NUM_BRANCHES][NUM_MESES];
    int total_sales[2];
    int total_profit[2];
};

typedef struct productInfo *ProductInfo;

ProductInfo createProductInfo(char* product){
    int i, j, k;
    ProductInfo pi = malloc(sizeof(struct productInfo));

    pi->product = my_strdup(product);
    for(i = 0; i < 2; i++){
        pi->total_sales[i] = 0;
        pi->total_profit[i] = 0;
        for(j = 0; j < NUM_BRANCHES; j++){
            for(k = 0; k < NUM_MESES; k++){
                pi->sales[i][j][k] = 0;
                pi->profit[i][j][k] = 0;
            }
        }
    }

    return pi;
}

void destroy_productInfo(void* ptr){
    ProductInfo pi = (ProductInfo) ptr;
    free(pi->product);
    free(pi);
}

char* piGetProduct(ProductInfo pi){
    return my_strdup(pi->product);
}

int piGetTotalSales(ProductInfo pi, char type){
    return pi->total_sales[type == 'P'];
}

float piGetTotalProfit(ProductInfo pi, char type){
    return pi->total_profit[type == 'P'];
}

void accumulateSaleOnInfo(ProductInfo pi, Venda v){
    int amount = getAmount(v);
    float profit = getPrice(v) * amount;
    int branch = getBranch(v);
    int month = getMonth(v);
    int index = getType(v) == 'P';

    pi->sales[index][branch-1][month-1] += amount;
    pi->profit[index][branch-1][month-1] += profit;
    pi->total_sales[index] += amount;
    pi->total_profit[index] += profit;
}

int piGetMonthlySalesInBranch(ProductInfo pi, char type, int branch, int month){
    return pi->sales[type == 'P'][branch-1][month-1];
}

int piGetMonthlySales(ProductInfo pi, char type, int month){
    int total = 0, i;
    for(i = 0; i < NUM_BRANCHES; i++) total += pi->sales[type=='P'][i][month-1];
    return total;
}

float piGetMonthlyProfit(ProductInfo pi, char type, int month){
    float total = 0;
    int i;
    for(i = 0; i < NUM_BRANCHES; i++) total += pi->profit[type=='P'][i][month-1];
    return total;
}

float piGetMonthlyProfitInBranch(ProductInfo pi, char type, int branch, int month){
    return pi->profit[type == 'P'][branch-1][month-1];
}

int piGetTotalSalesInBranch(ProductInfo pi, char type, int branch){
    int total = 0, i;
    for(i = 0; i < NUM_MESES; i++) total += pi->sales[type == 'P'][branch-1][i];
    return total;
}

float piGetTotalProfitInBranch(ProductInfo pi, char type, int branch){
    float profit = 0;
    int i;
    for(i = 0; i < NUM_MESES; i++) profit += pi->profit[type == 'P'][branch-1][i];
    return profit;
}

struct faturacao {
    Catalog salesCatalog;
};

/* Initializes the structure */
Faturacao init_faturacao(){
    Faturacao f = malloc(sizeof(struct faturacao));

    f->salesCatalog = init_catalog(NUM_LETTERS, PRODUCTS_DEPTH, 9000, 1000, destroy_productInfo);

    return f;
}

/* Destroys the faturacao and free's the memory it retained */
void destroy_faturacao(Faturacao f){
    destroy_catalog(f->salesCatalog);
    free(f);
}

/* Bills a product */
void billProduct(Faturacao f, Venda v){
    char* product = getProduct(v);

    ProductInfo pi = (ProductInfo) catalog_get(f->salesCatalog, product);
    accumulateSaleOnInfo(pi, v);

    free(product);
}

int wasSold(Faturacao f, char* product){
    ProductInfo pi = catalog_get(f->salesCatalog, product);

    return !(piGetTotalSales(pi,'N') || piGetTotalSales(pi,'P'));
}

int wasSoldInBranch(Faturacao f, char* product, int branch){
    ProductInfo pi = catalog_get(f->salesCatalog, product);

    return !(piGetTotalSalesInBranch(pi, 'N', branch) || piGetTotalSalesInBranch(pi, 'P', branch));
}

void addEmptyProductToFac(Faturacao f, char* product){
    ProductInfo pi = createProductInfo(product);
    catalog_insert(f->salesCatalog, product, pi);
}

int getProductMonthlySalesInBranch(Faturacao f, char* product, char type, int branch, int month){
    ProductInfo pi = catalog_get(f->salesCatalog, product);
    return piGetMonthlySalesInBranch(pi, type, branch, month);
}

float getProductMonthlyProfitInBranch(Faturacao f, char* product, char type, int branch, int month){
    ProductInfo pi = catalog_get(f->salesCatalog, product);
    return piGetMonthlyProfitInBranch(pi, type, branch, month);
}

gboolean sumMonthlySales(gpointer key, gpointer value, gpointer data){
    ProductInfo pi = (ProductInfo) value;
    int* array = ((int*) data);
    int month = array[0];

    array[1] += (piGetMonthlySales(pi, 'P', month) + piGetMonthlySales(pi, 'N', month));

    return FALSE;
}

int getMonthlySales(Faturacao f, int month){
    /* Holds the month in index 0 and the value we want in index 1 */
    int* array = malloc(sizeof(int) * 2);
    array[0] = month;
    array[1] = 0;

    catalog_foreach(f->salesCatalog, (GTraverseFunc) sumMonthlySales, array);

    int monthlySales = array[1];
    free(array);

    return monthlySales;
}

gboolean sumMonthlyProfit(gpointer key, gpointer value, gpointer data){
    ProductInfo pi = (ProductInfo) value;
    Pair p = (Pair) data;
    int month = *((int*) fst(p));
    float* profit = (float*) snd(p);

    *profit += (piGetMonthlyProfit(pi, 'P', month) + piGetMonthlyProfit(pi, 'N', month));

    return FALSE;
}

float getMonthlyProfit(Faturacao f, int month){
    int *month_ptr = malloc(sizeof(int)); *month_ptr = month;
    float *profit = malloc(sizeof(float)); *profit = 0;
    Pair p = mkPair(month_ptr, profit);

    catalog_foreach(f->salesCatalog, (GTraverseFunc) sumMonthlyProfit, p);

    float profit_ret = *profit;
    free(month_ptr); free(profit); freePair(p);

    return profit_ret;
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
