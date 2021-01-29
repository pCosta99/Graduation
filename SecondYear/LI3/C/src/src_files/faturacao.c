#include "pair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "faturacao.h"

struct productInfo{
    char* product;
    int*** sales;
    float*** profit;
    int* total_sales;
    int* total_profit;
};

typedef struct productInfo *ProductInfo;

GPtrArray* ProductInfoToString(ProductInfo pi){
    GPtrArray* array = g_ptr_array_new_with_free_func(free);
    char *string = malloc(sizeof(char) * 200), *append = malloc(sizeof(char) * 20);
    int i, j, k;
    int control;

    sprintf(string, "Código: %s\n", pi->product); g_ptr_array_add(array, my_strdup(string));
    for(i = 0; i < 2; i++){
        /* Inform about the type */
        if(i) { sprintf(string, "\nEm promoção: "); g_ptr_array_add(array, my_strdup(string)); }
        else { sprintf(string, "Regular: "); g_ptr_array_add(array, my_strdup(string)); }

        for(j = 0; j < get_num_branches(); j++){
            /* Inform about the branch */
            sprintf(string, "Filial %d", j+1);

            /* Clear the buffer */
            memset(string, 0, 200);

            for(control = 0; control < 2; control++){
                /* State what we will be printing */
                if(control == 1) strcat(string, "Vendas: ");
                else strcat(string, "Lucros: ");

                for(k = 0; k < get_num_meses(); k++){
                    if(control) sprintf(append, "%d ", pi->sales[i][j][k]);
                    else sprintf(append, "%.2f ", pi->profit[i][j][k]);
                    strcat(string, append);
                }
                g_ptr_array_add(array, my_strdup(string));
                /* Clear the buffer */
                memset(string, 0, 200);
            }
        }
    }

    free(string); free(append);

    return array;
}

ProductInfo createProductInfo(char* product){
    int i, j, k;
    ProductInfo pi = malloc(sizeof(struct productInfo));

    pi->product = my_strdup(product);
    pi->total_sales = malloc(sizeof(int) * get_num_types());
    pi->total_profit = malloc(sizeof(float) * get_num_types());
    pi->sales = malloc(sizeof(int**) * get_num_types());
    pi->profit = malloc(sizeof(float**) * get_num_types());
    for(i = 0; i < get_num_types(); i++){
        pi->total_sales[i] = 0;
        pi->total_profit[i] = 0;
        pi->sales[i] = malloc(sizeof(int*) * get_num_branches());
        pi->profit[i] = malloc(sizeof(float*) * get_num_branches());
        for(j = 0; j < get_num_branches(); j++){
            pi->sales[i][j] = malloc(sizeof(int) * get_num_meses());
            pi->profit[i][j] = malloc(sizeof(float) * get_num_meses());
            for(k = 0; k < get_num_meses(); k++){
                pi->sales[i][j][k] = 0;
                pi->profit[i][j][k] = 0;
            }
        }
    }

    return pi;
}

void destroy_productInfo(ProductInfo pi){
    int i, j;
    free(pi->product);
    for(i = 0; i < get_num_types(); i++){
        for(j = 0; j < get_num_branches(); j++){
            free(pi->sales[i][j]);
            free(pi->profit[i][j]);
        }
        free(pi->sales[i]);
        free(pi->profit[i]);
    }
    free(pi->sales);
    free(pi->profit);
    free(pi->total_sales);
    free(pi->total_profit);
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
    for(i = 0; i < get_num_branches(); i++) total += pi->sales[type=='P'][i][month-1];
    return total;
}

float piGetMonthlyProfit(ProductInfo pi, char type, int month){
    float total = 0;
    int i;
    for(i = 0; i < get_num_branches(); i++) total += pi->profit[type=='P'][i][month-1];
    return total;
}

float piGetMonthlyProfitInBranch(ProductInfo pi, char type, int branch, int month){
    return pi->profit[type == 'P'][branch-1][month-1];
}

int piGetTotalSalesInBranch(ProductInfo pi, char type, int branch){
    int total = 0, i;
    for(i = 0; i < get_num_meses(); i++) total += pi->sales[type == 'P'][branch-1][i];
    return total;
}

float piGetTotalProfitInBranch(ProductInfo pi, char type, int branch){
    float profit = 0;
    int i;
    for(i = 0; i < get_num_meses(); i++) profit += pi->profit[type == 'P'][branch-1][i];
    return profit;
}

struct faturacao {
    Catalog salesCatalog;
};

/* Initializes the structure */
Faturacao init_faturacao(){
    Faturacao f = malloc(sizeof(struct faturacao));

    f->salesCatalog = init_catalog(get_num_letters(), get_products_depth(), compare_ints, free, destroy_productInfo);

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
    long* array = ((long*) data);
    int month = (int) array[0];

    array[1] += (long) (piGetMonthlySales(pi, 'P', month) + piGetMonthlySales(pi, 'N', month));

    return FALSE;
}

long getMonthlySales(Faturacao f, int month){
    /* Holds the month in index 0 and the value we want in index 1 */
    long* array = malloc(sizeof(long) * 2);
    array[0] = (long) month;
    array[1] = 0;

    catalog_foreach(f->salesCatalog, (GTraverseFunc) sumMonthlySales, array);

    long monthlySales = array[1];
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

GPtrArray* productInfoAsString(Faturacao f, char* product){
    ProductInfo pi = (ProductInfo) catalog_get(f->salesCatalog, product);

    return ProductInfoToString(pi);
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
