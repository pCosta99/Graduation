#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "parser.h"
#include "sgv.h"
#include "pqueue.h"
#include "pair.h"

#define PATH_STATS         "Stats"
#define PATH_VENDAS		   "Vendas_1M.txt"
#define PATH_CLIENTES	   "Clientes.txt"
#define PATH_PRODS		   "Produtos.txt"
#define LINE_SIZE_PRODS    6
#define LINE_SIZE_CLIENTES 5
#define LINE_SIZE_VENDAS   40
#define NUM_BRANCHES       3
#define NUM_MESES          12

struct sgv{
    CatProds products;
    CatClientes customers;
    Faturacao f;
    Filial branches[NUM_BRANCHES];
    Stats stats;

    int validSales, totalSales;
    char *salesPath, *productsPath, *customersPath;
};

SGV initSGV(){
    int i;
    SGV sgv = malloc(sizeof(struct sgv));
    sgv->products = initCatProds();
    sgv->customers = initCatClientes();
    sgv->f = init_faturacao();
    for(i = 0; i < NUM_BRANCHES; i++) sgv->branches[i] = init_filial();
    sgv->stats = init_stats(PATH_STATS, 1, 1);
    return sgv;
}

SGV initSGVWithTimingOn(){
    int i;
    SGV sgv = malloc(sizeof(struct sgv));
    sgv->products = initCatProds();
    sgv->customers = initCatClientes();
    sgv->f = init_faturacao();
    for(i = 0; i < NUM_BRANCHES; i++) sgv->branches[i] = init_filial();
    sgv->stats = init_stats(PATH_STATS, 0, 0);
    return sgv;
}

void destroySGV(SGV sgv){
    int i;
    destroyCatProds(sgv->products);
    destroyCatClientes(sgv->customers);
    destroy_faturacao(sgv->f);
    for(i = 0; i < NUM_BRANCHES; i++) destroy_filial(sgv->branches[i]);
    destroy_stats(sgv->stats);

    free(sgv->salesPath);
    free(sgv->productsPath);
    free(sgv->customersPath);
    free(sgv);
}

SGV loadSGVFromFolder(SGV sgv, char* filesFolderPath){
    char* totalPath = malloc(sizeof(char) * 100);

    sprintf(totalPath, "%s/%s", filesFolderPath, PATH_PRODS);
    sgv->productsPath = my_strdup(totalPath);
    loadCatalog(sgv, totalPath, LINE_SIZE_PRODS, 0);

    sprintf(totalPath, "%s/%s", filesFolderPath, PATH_CLIENTES);
    sgv->customersPath = my_strdup(totalPath);
    loadCatalog(sgv, totalPath, LINE_SIZE_CLIENTES, 1);

    sprintf(totalPath, "%s/%s", filesFolderPath, PATH_VENDAS);
    sgv->salesPath = my_strdup(totalPath);
    loadVendas(sgv, totalPath, LINE_SIZE_VENDAS);

    free(totalPath);

    return sgv;
}

SGV loadSGVFromFiles(SGV sgv, char* customersPath, char* productsPath, char* salesPath){
    sgv->productsPath = my_strdup(productsPath); loadCatalog(sgv, productsPath, LINE_SIZE_PRODS, 0);
    sgv->customersPath = my_strdup(customersPath); loadCatalog(sgv, customersPath, LINE_SIZE_CLIENTES, 1);
    sgv->salesPath = my_strdup(salesPath); loadVendas(sgv, salesPath, LINE_SIZE_VENDAS);

    return sgv;
}

/* ----------------------- SGV INTERFACE ----------------------------- */
void setValidSales(SGV sgv, int validSales){
    sgv->validSales = validSales;
}

void setTotalSales(SGV sgv, int totalSales){
    sgv->totalSales = totalSales;
}

void sgv_setValidProducts(SGV sgv, int validProducts){
    setValidProducts(sgv->products, validProducts);
}

void sgv_setTotalProducts(SGV sgv, int totalProducts){
    setTotalProducts(sgv->products, totalProducts);
}

void sgv_setValidCustomers(SGV sgv, int validCustomers){
    setValidCustomers(sgv->customers, validCustomers);
}

void sgv_setTotalCustomers(SGV sgv, int totalCustomers){
    setTotalCustomers(sgv->customers, totalCustomers);
}

void sgv_addProductToCatalog(SGV sgv, char* product){
    insertProduct(sgv->products, product);
}

int sgv_checkIfProductExists(SGV sgv, char* product){
    return productExists(sgv->products, product);
}

void sgv_addCustomerToCatalog(SGV sgv, char* customer){
    insertCustomer(sgv->customers, customer);
}

int sgv_checkIfCustomerExists(SGV sgv, char* customer){
    return customerExists(sgv->customers, customer);
}

void sgv_addEmptyProductToFac(SGV sgv, char* product){
    addEmptyProductToFac(sgv->f, product);
}

void sgv_billProduct(SGV sgv, Venda v){
    billProduct(sgv->f, v);
}

void sgv_addEmptyCustomerToBranches(SGV sgv, char* customer){
    int i;
    for(i = 0; i < NUM_BRANCHES; i++) addEmptyCustomerToBranch(sgv->branches[i], customer);
}

void sgv_customerBought(SGV sgv, Venda v){
    int branch = getBranch(v);
    customerBought(sgv->branches[branch-1], v);
}

Stats getStats(SGV sgv){
    return sgv->stats;
}

/* --------------------------- QUERIES ------------------------------- */

/* --------------------------- QUERY 2 ------------------------------- */
void free_gptrarray(void* ptr){
    GPtrArray* array = (GPtrArray*) ptr;
    g_ptr_array_set_free_func(array, free);
    g_ptr_array_free (array, TRUE);
}

gboolean end_with_newline(gpointer data, gpointer user_data){
    GPtrArray* array = (GPtrArray*) user_data;
    char* product = (char*) data;
    char* string = malloc(sizeof(char) * 20);
    sprintf(string, "%s\n", product);

    g_ptr_array_add(array, my_strdup(string));
    free(string);

    return FALSE;
}

PrintableResults getProductsStartedByLetter(SGV sgv, char letter){
    GPtrArray* printable = g_ptr_array_new_with_free_func(free);
    start_timing(sgv->stats, "query2");

    GPtrArray* products = retrieve_letter_products(sgv->products, letter);
    g_ptr_array_foreach(products, (GFunc) end_with_newline, printable);
    g_ptr_array_free(products, TRUE);

    finish_timing(sgv->stats, "query2");

    PrintableResults pr = initPrintableResults(printable, NULL, free_gptrarray, 1);

    return pr;
}

/* --------------------------- QUERY 3 ------------------------------- */
struct query3{
    int vendas_N;
    int vendas_P;
    float profit_N;
    float profit_P;
};

typedef struct query3 *Query3;

GPtrArray* print_query3(void* ptr){
    Query3 q = (Query3) ptr;
    GPtrArray* ret = g_ptr_array_new();

    char* data = malloc(sizeof(char) * 100);
    sprintf(data, "Número de vendas normais: %d\n", q->vendas_N);
    g_ptr_array_add(ret, my_strdup(data));
    sprintf(data, "Número de vendas em promoção: %d\n", q->vendas_P);
    g_ptr_array_add(ret, my_strdup(data));
    sprintf(data, "Total faturado fora de promoção: %.2f\n", q->profit_N);
    g_ptr_array_add(ret, my_strdup(data));
    sprintf(data, "Total faturado em promoção: %.2f\n", q->profit_P);
    g_ptr_array_add(ret, data);

    return ret;
}

void free_query3(void* ptr){
    Query3 q = (Query3) ptr;
    free(q);
}

Query3 getProductSalesAndProfitInBranch(SGV sgv, char* productID, int month, int branch){
    Query3 q = malloc(sizeof(struct query3));

    q->vendas_N = getProductMonthlySalesInBranch(sgv->f, productID, 'N', branch, month);
    q->profit_N = getProductMonthlyProfitInBranch(sgv->f, productID, 'N', branch, month);
    q->vendas_P = getProductMonthlySalesInBranch(sgv->f, productID, 'P', branch, month);
    q->profit_P = getProductMonthlyProfitInBranch(sgv->f, productID, 'P', branch, month);

    return q;
}

 /*Agreggate tells us if we want the result branch by branch or gathered*/
PrintableResults getProductSalesAndProfit(SGV sgv, char* productID, int month, int agreggate){
    Stats s = getStats(sgv);
    int i;
    PrintableResults pr;

    start_timing(s, "query3");

    Query3* branchs = malloc(sizeof(struct query3) * NUM_BRANCHES);
    for(i = 0; i < NUM_BRANCHES; i++){
        branchs[i] = getProductSalesAndProfitInBranch(sgv, productID, month, i+1);
    }
    Query3 total = malloc(sizeof(struct query3));
    total->vendas_N = 0; total->vendas_P = 0; total->profit_N = 0; total->profit_P = 0;
    for(i = 0; i < NUM_BRANCHES; i++){
        total->vendas_N += branchs[i]->vendas_N;
        total->vendas_P += branchs[i]->vendas_P;
        total->profit_N += branchs[i]->profit_N;
        total->profit_P += branchs[i]->profit_P;
    }

    if(agreggate){
        for(i = 0; i < NUM_BRANCHES; i++) free(branchs[i]);
        free(branchs);
        pr = initPrintableResults(total, print_query3, free_query3, 0);
    } else {
        free(total);
        pr = initMultiplePrintableResults((void**) branchs, print_query3, free_query3, 0, NUM_BRANCHES);
        free(branchs);
    }

    finish_timing(s, "query3");

    return pr;
}

/* --------------------------- QUERY 4 ------------------------------- */
struct query4{
    GPtrArray* array;
    Faturacao f;
    int branchToCheck;
};

typedef struct query4 *Query4;

void free_query4(void* ptr){
    Query4 q = (Query4) ptr;

    g_ptr_array_free(q->array, TRUE);
    free(q);
}

GPtrArray* print_query4(void* ptr){
    Query4 q = (Query4) ptr;

    return q->array;
}

gboolean insert_in_array2(gpointer data, gpointer user_data){
    Query4 q = (Query4) user_data;
    char* string = malloc(sizeof(char) * 20);
    sprintf(string, "%s\n", (char*) data);

    if(q->branchToCheck && wasSoldInBranch(q->f, (char*) data, q->branchToCheck)) g_ptr_array_add(q->array, my_strdup(string));
    else if(wasSold(q->f, (char*) data)) g_ptr_array_add(q->array, my_strdup(string));

    free(string);

    return FALSE;
}

PrintableResults getProductsNeverBought(SGV sgv, int oneByOne){
    PrintableResults pr = NULL;
    int i;

    start_timing(sgv->stats, "query4");
    GPtrArray* products = allProducts(sgv->products);

    if(oneByOne){
        Query4* oneByOneQuery = malloc(sizeof(Query4) * NUM_BRANCHES);
        for(i = 0; i < NUM_BRANCHES; i++) {
            oneByOneQuery[i] = malloc(sizeof(struct query4));
            oneByOneQuery[i]->array = g_ptr_array_new_with_free_func(free);
            oneByOneQuery[i]->f = sgv->f;
            oneByOneQuery[i]->branchToCheck = i+1;
        }

        for(i = 0; i < NUM_BRANCHES; i++) g_ptr_array_foreach(products, (GFunc) insert_in_array2, oneByOneQuery[i]);

        pr = initMultiplePrintableResults((void**) oneByOneQuery, print_query4, free_query4, 0, NUM_BRANCHES);
        free(oneByOneQuery);
    } else {
        Query4 q = malloc(sizeof(struct query4));
        q->array = g_ptr_array_new_with_free_func(free); q->f = sgv->f; q->branchToCheck = 0;

        g_ptr_array_foreach(products, (GFunc) insert_in_array2, q);

        pr = initPrintableResults(q, print_query4, free_query4, 0);
    }
    finish_timing(sgv->stats, "query4");
    g_ptr_array_free(products, TRUE);

    return pr;
}

/* --------------------------- QUERY 5 ------------------------------- */
struct query5{
    GPtrArray* array;
    SGV sgv;
};

typedef struct query5 *Query5;

void free_query5(void* ptr){
    Query5 q = (Query5) ptr;

    g_ptr_array_free(q->array, TRUE);
    free(q);
}

void insert_in_array_query5(gpointer data, gpointer user_data){
    Query5 q = (Query5) user_data;
    char* customer = (char*) data;
    int i, bought = 0;

    for(i = 0; i < NUM_BRANCHES; i++) bought += didCustomerBought(q->sgv->branches[i], customer);

    char* string = malloc(sizeof(char) * 10);
    sprintf(string, "%s\n", customer);
    if(!bought) g_ptr_array_add(q->array, my_strdup(string));

    free(string);
}

PrintableResults getClientsOfAllBranches(SGV sgv){
    GPtrArray* customers = allCustomers(sgv->customers);
    Query5 q = malloc(sizeof(struct query5));
    q->array = g_ptr_array_new_with_free_func(free);
    q->sgv = sgv;

    start_timing(sgv->stats, "query5");
    g_ptr_array_foreach(customers, (GFunc) insert_in_array_query5, q);
    finish_timing(sgv->stats, "query5");

    PrintableResults pr = initPrintableResults(q->array, NULL, free_gptrarray, 1);
    free(q);
    g_ptr_array_free(customers, TRUE);

    return pr;
}

/* --------------------------- QUERY 6 ------------------------------- */
struct query6{
    int productsNeverBought;
    int customersThatNeverBought;
    SGV sgv;
    GPtrArray* array_to_print;
};

typedef struct query6 *Query6;

void free_query6(void* ptr){
    Query6 q = (Query6) ptr;
    g_ptr_array_free(q->array_to_print, TRUE);
    free(ptr);
}

GPtrArray* print_query6(void* ptr){
    Query6 q = (Query6) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);

    char* data = malloc(sizeof(char) * 100);
    sprintf(data, "Número de produtos que ninguém comprou: %d\n", q->productsNeverBought);
    g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Número de clientes que nunca compraram: %d\n", q->customersThatNeverBought);
    g_ptr_array_add(q->array_to_print, my_strdup(data));

    free(data);

    return q->array_to_print;
}

gboolean countNotSold(gpointer data, gpointer user_data){
    char* product = (char*) data;
    Query6 q = (Query6) user_data;

    if(wasSold(q->sgv->f, product)) q->productsNeverBought++;

    return FALSE;
}

gboolean countDidntBuy(gpointer data, gpointer user_data){
    char* customer = (char*) data;
    Query6 q = (Query6) user_data;
    int i, bought = 0;

    for(i = 0; i < NUM_BRANCHES; i++) bought += didCustomerBought(q->sgv->branches[i], customer);
    if(bought) q->customersThatNeverBought++;

    return FALSE;
}

PrintableResults getClientsAndProductsNeverBoughtCount(SGV sgv){
    Query6 q = malloc(sizeof(struct query6));
    q->productsNeverBought = 0;
    q->customersThatNeverBought = 0;
    q->sgv = sgv;

    start_timing(sgv->stats, "query6");
    GPtrArray* products = allProducts(sgv->products);
    GPtrArray* customers = allCustomers(sgv->customers);

    g_ptr_array_foreach(products, (GFunc) countNotSold, q);
    g_ptr_array_foreach(customers, (GFunc) countDidntBuy, q);
    finish_timing(sgv->stats, "query6");

    g_ptr_array_free(products, TRUE);
    g_ptr_array_free(customers, TRUE);

    PrintableResults pr = initPrintableResults(q, print_query6, free_query6, 0);
    return pr;
}

/* --------------------------- QUERY 7 ------------------------------- */
struct query7{
    int** monthlyAmounts;
    GPtrArray* array_to_print;
};

typedef struct query7 *Query7;

void free_query7(void* ptr){
    int i;
    Query7 q = (Query7) ptr;
    for(i = 0; i < NUM_BRANCHES; i++) free(q->monthlyAmounts[i]);
    free(q->monthlyAmounts);
    g_ptr_array_free(q->array_to_print, TRUE);
    free(q);
}

GPtrArray* print_query7(void* ptr){
    Query7 q = (Query7) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);
    int i, j;

    char* data = malloc(sizeof(char) * 100);
    for(i = 0; i < NUM_BRANCHES; i++){
        for(j = 0; j < NUM_MESES; j++){
            sprintf(data, "%d ", q->monthlyAmounts[i][j]);
            g_ptr_array_add(q->array_to_print, my_strdup(data));
        }
        sprintf(data, "\n");
        g_ptr_array_add(q->array_to_print, my_strdup(data));
    }
    free(data);

    return q->array_to_print;
}

PrintableResults getProductsBoughtByClient(SGV sgv, char* clientID){
    Query7 q = malloc(sizeof(struct query7));
    q->monthlyAmounts = malloc(sizeof(int*) * NUM_BRANCHES);
    int i, j;

    start_timing(sgv->stats, "query7");

    for(i = 0; i < NUM_BRANCHES; i++){
        q->monthlyAmounts[i] = malloc(sizeof(int) * NUM_MESES);
        for(j = 0; j < NUM_MESES; j++) q->monthlyAmounts[i][j] = 0;
        getProductsBoughtByClientInBranch(sgv->branches[i], q->monthlyAmounts[i], clientID);
    }

    finish_timing(sgv->stats, "query7");

    PrintableResults pr = initPrintableResults(q, print_query7, free_query7, 0);

    return pr;
}

/* --------------------------- QUERY 8 ------------------------------- */
struct query8{
    int total_vendas;
    float total_faturado;
    GPtrArray* array_to_print;
};

typedef struct query8 *Query8;

void free_query8(void* ptr){
    Query8 q = (Query8) ptr;
    g_ptr_array_free(q->array_to_print, TRUE);
    free(ptr);
}

GPtrArray* print_query8(void* ptr){
    Query8 q = (Query8) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);

    char* data = malloc(sizeof(char) * 100);
    sprintf(data, "Total faturado: %.2f\n", q->total_faturado);
    g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Total vendido: %d\n", q->total_vendas);
    g_ptr_array_add(q->array_to_print, my_strdup(data));

    free(data);

    return q->array_to_print;
}

PrintableResults getSalesAndProfit(SGV sgv, int minMonth, int maxMonth){
    int i;

    Query8 q = malloc(sizeof(struct query8));
    q->total_faturado = 0;
    q->total_vendas = 0;

    start_timing(sgv->stats, "query8");

    for(i = minMonth; i <= maxMonth; i++){
        q->total_faturado += getMonthlyProfit(sgv->f, i);
        q->total_vendas += getMonthlySales(sgv->f, i);
    }

    finish_timing(sgv->stats, "query8");

    PrintableResults pr = initPrintableResults(q, print_query8, free_query8, 0);
    return pr;
}
/* --------------------------- QUERY 9 ------------------------------- */
struct query9{
    GPtrArray* regular;
    GPtrArray* promotion;
    GPtrArray* array_to_print;
};

typedef struct query9 *Query9;

void free_query9(void* ptr){
    Query9 q = (Query9) ptr;
    g_ptr_array_free(q->regular, TRUE);
    g_ptr_array_free(q->promotion, TRUE);
    g_ptr_array_free(q->array_to_print, TRUE);
    free(q);
}

GPtrArray* print_query9(void* ptr){
    int i;
    Query9 q = (Query9) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(g_free);

    char* data = malloc(sizeof(char) * 100);
    sprintf(data, "Em modo normal:\n");
    g_ptr_array_add(q->array_to_print, my_strdup(data));
    for(i = 0; i < q->regular->len; i++) {
        sprintf(data, "%s\n", (char*) g_ptr_array_index(q->regular, i));
        g_ptr_array_add(q->array_to_print, my_strdup(data));
    }
    sprintf(data, "\nEm promoção:\n");
    g_ptr_array_add(q->array_to_print, my_strdup(data));
    for(i = 0; i < q->promotion->len; i++) {
        sprintf(data, "%s\n", (char*) g_ptr_array_index(q->promotion, i));
        g_ptr_array_add(q->array_to_print, my_strdup(data));
    }
    free(data);

    return q->array_to_print;
}

PrintableResults getProductBuyers(SGV sgv, char* productID, int branch){
    Query9 q = malloc(sizeof(struct query9));

    start_timing(sgv->stats, "query9");

    GPtrArray** arrays = getProductBuyersInBranch(sgv->branches[branch-1], productID);
    q->regular = arrays[0];
    q->promotion = arrays[1];
    free(arrays);

    finish_timing(sgv->stats, "query9");
    PrintableResults pr = initPrintableResults(q, print_query9, free_query9, 0);

    return pr;
}

/* --------------------------- QUERY 10 ------------------------------ */
struct query10{
    PQueue* min_heap;
    GPtrArray* array_to_print;
};
typedef struct query10 *Query10;

GPtrArray* print_query10(void* ptr){
    Query10 q = (Query10) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 100);

    char* n = (char*) pqueue_dequeue(q->min_heap);
    while(n){
        sprintf(data, "%s\n", n);
        g_ptr_array_add(q->array_to_print, my_strdup(data));
        free(n);
        n = (char*) pqueue_dequeue(q->min_heap);
    }
    free(data);
    free(n);

    return q->array_to_print;
}

void free_query10(void* ptr){
    Query10 q = (Query10) ptr;

    pqueue_delete(q->min_heap);
    g_ptr_array_free(q->array_to_print, TRUE);
    free(q);
}

gboolean tableToHeap(gpointer key, gpointer value, gpointer data){
    Query10 q = (Query10) data;
    char* product = my_strdup((char*) key);
    int* productAmount = malloc(sizeof(int)); *productAmount = *((int*) value);

    pqueue_enqueue(q->min_heap, productAmount, product);

    return FALSE;
}

PrintableResults getClientsFavouriteProducts(SGV sgv, char* clientID, int month){
    int i;
    Query10 q = malloc(sizeof(struct query10));
    q->min_heap = pqueue_new(compare_ints_two_args, free, free, 1000000);
    GHashTable* table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    start_timing(sgv->stats, "query10");
    /* Fully gather the table */
    for(i = 0; i < NUM_BRANCHES; i++) {
        getClientFavProducts(sgv->branches[i], clientID, month, table);
    }

    /* Turn it into a heap */
    g_hash_table_foreach(table, (GHFunc) tableToHeap, q);
    g_hash_table_destroy(table);
    finish_timing(sgv->stats, "query10");

    PrintableResults pr = initPrintableResults(q, print_query10, free_query10, 0);
    return pr;
}

/* --------------------------- QUERY 11 ------------------------------ */
struct query11{
    PQueue* max_heap;
    int number_to_extract;
    GPtrArray* array_to_print;
};
typedef struct query11 *Query11;

void recursive_depth1_free(void* f){
    void** ptr = (void**) f;
    int i;
    for(i = 0; ptr[i]; i++) free(ptr[i]);
    free(ptr);
}

GPtrArray* print_query11(void* ptr){
    int counter = 0;
    Query11 q = (Query11) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 500);

    Pair p = (Pair) pqueue_dequeue(q->max_heap);
    while(p){
        char* product = (char*) fst(p);
        int** product_info = (int**) snd(p);
        if(counter < q->number_to_extract){
            sprintf(data, "Product: %s, n_buyers: %d, n_buyers_1: %d, n_buyers_2: %d, n_buyers_3 %d, amount: %d, amount_1 %d, amount_2: %d, amount_3: %d", product, product_info[0][0], product_info[1][0], product_info[2][0], product_info[3][0], product_info[0][1], product_info[1][1], product_info[2][1], product_info[3][1]);
            if(counter + 1 < q->number_to_extract) strcat(data, "\n");
            g_ptr_array_add(q->array_to_print, my_strdup(data));
        }
        free(product); recursive_depth1_free(product_info); freePair(p);
        p = (Pair) pqueue_dequeue(q->max_heap);
        counter++;
    }
    free(data);
    freePair(p);

    return q->array_to_print;
}

void free_query11(void* ptr){
    Query11 q = (Query11) ptr;

    pqueue_delete(q->max_heap);
    g_ptr_array_free(q->array_to_print, TRUE);
    free(q);
}

gboolean tableToHeap_Query11(gpointer key, gpointer value, gpointer data){
    Query11 q = (Query11) data;
    char* product = my_strdup((char*) key);
    int **array = ((int**) value);

    /* Clone the value we have in the table */
    int **product_info_clone = malloc(sizeof(int*) * (NUM_BRANCHES + 2)), i;
    for(i = 0; i < NUM_BRANCHES + 1; i++){
        product_info_clone[i] = malloc(sizeof(int) * 2);
        if(array[i]) {
            product_info_clone[i][0] = array[i][0];
            product_info_clone[i][1] = array[i][1];
        } else {
            product_info_clone[i][0] = 0;
            product_info_clone[i][1] = 0;
        }
    }
    product_info_clone[i] = 0;
    /* Get the total amount purchased */
    int* amountPurchased = malloc(sizeof(int));
    *amountPurchased = array[0][1];

    /* This pair corresponds to the value we are going to have on the heap */
    Pair p = mkPair(product, product_info_clone);

    /* We use the amountPurchased as key (that's the sorting criteria) */
    pqueue_enqueue(q->max_heap, amountPurchased, p);

    return FALSE;
}

PrintableResults getTopSelledProducts(SGV sgv, int limit){
    int i;
    Query11 q = malloc(sizeof(struct query11));
    q->max_heap = pqueue_new(compare_ints_two_args, free, free, 1000000);
    q->number_to_extract = limit;
    GHashTable* hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, recursive_depth1_free);
    int* info = malloc(sizeof(int) * 2); /* index 0 is num_branches, index 1 is current */
    info[0] = NUM_BRANCHES;

    start_timing(sgv->stats, "query11");

    /* Pass the empty array directly into the branchs to fill it up */
    for(i = 0; i < NUM_BRANCHES; i++){
        info[1] = i+1;
        char* string = malloc(sizeof(char) * 100);
        sprintf(string, "query11_loadTree%d", i+1);
        start_timing(sgv->stats, string);

        getSoldProductsInBranch(sgv->branches[i], hashtable, info);

        finish_timing(sgv->stats, string);
        free(string);
    }

    /* Turn it into a max-heap */
    start_timing(sgv->stats, "query11_treeToHeap");
    g_hash_table_foreach(hashtable, (GHFunc) tableToHeap_Query11, q);
    finish_timing(sgv->stats, "query11_treeToHeap");

    g_hash_table_destroy(hashtable);
    free(info);
    finish_timing(sgv->stats, "query11");

    PrintableResults pr = initPrintableResults(q, print_query11, free_query11, 0);
    return pr;
}

/* --------------------------- QUERY 12 ------------------------------ */
struct query12{
    PQueue* max_heap;
    int limit; /* number of items to print */
    GPtrArray* array_to_print;
};

typedef struct query12 *Query12;

GPtrArray* print_query12(void* ptr){
    Query12 q = (Query12) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 100);
    int counter = 0;

    Pair p = (Pair) pqueue_dequeue(q->max_heap);
    while(p){
        char* product = fst(p);
        float* amount_bought = snd(p);
        sprintf(data, "%s : %.2f\n", product, *amount_bought);
        if(counter < q->limit) g_ptr_array_add(q->array_to_print, my_strdup(data));
        free(product); free(amount_bought); freePair(p);
        p = (Pair) pqueue_dequeue(q->max_heap);
        counter++;
    }
    free(data);
    freePair(p);

    return q->array_to_print;
}

void free_query12(void* ptr){
    Query12 q = (Query12) ptr;

    pqueue_delete(q->max_heap);
    g_ptr_array_free(q->array_to_print, TRUE);
    free(q);
}

gboolean tableToHeap_Query12(gpointer key, gpointer value, gpointer data){
    Query12 q = (Query12) data;
    char* product = my_strdup((char*) key);

    /* Do two copies so we can have it stored in the value too */
    float *amount_bought_key = malloc(sizeof(float)); *amount_bought_key = *((float*) value);
    float *amount_bought_value = malloc(sizeof(float)); *amount_bought_value = *((float*) value);

    /* Setup the pair to go into the value field */
    Pair p = mkPair(product, amount_bought_value);

    /* We use the amountPurchased as key (that's the sorting criteria) */
    pqueue_enqueue(q->max_heap, amount_bought_key, p);

    return FALSE;
}

PrintableResults getClientTopProfitProducts(SGV sgv, char* clientID, int limit){
    int i;
    Query12 q = malloc(sizeof(struct query12));
    q->limit = limit;
    q->max_heap = pqueue_new(compare_floats_two_args, free, free, 1000000);
    GHashTable* table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    start_timing(sgv->stats, "query12");
    /* Fully gather the table */
    for(i = 0; i < NUM_BRANCHES; i++) {
        getClientTopProfitProductsBranch(sgv->branches[i], clientID, table);
    }

    /* Turn it into a heap */
    g_hash_table_foreach(table, (GHFunc) tableToHeap_Query12, q);
    g_hash_table_destroy(table);
    finish_timing(sgv->stats, "query12");

    PrintableResults pr = initPrintableResults(q, print_query12, free_query12, 0);
    return pr;
}

/* --------------------------- QUERY 13 ------------------------------ */
struct query13{
    char* salesPath;
    int validSales, totalSales;
    char* customersPath;
    int validCustomers, totalCustomers;
    char* productsPath;
    int validProducts, totalProducts;
    GPtrArray* array_to_print;
};

typedef struct query13 *Query13;

GPtrArray* print_query13(void* ptr){
    Query13 q = (Query13) ptr;
    q->array_to_print = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 200);

    /* Sales info */
    sprintf(data, "Vendas:\n"); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Ficheiro: %s\n", q->salesPath); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Vendas válidas: %d\n", q->validSales); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Vendas inválidas: %d\n", (q->totalSales - q->validSales)); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Total de vendas: %d\n", q->totalSales); g_ptr_array_add(q->array_to_print, my_strdup(data));

    /* Products info */
    sprintf(data, "Produtos:\n"); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Ficheiro: %s\n", q->productsPath); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Produtos válidos: %d\n", q->validProducts); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Produtos inválidos: %d\n", (q->totalProducts - q->validProducts)); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Total de produtos: %d\n", q->totalProducts); g_ptr_array_add(q->array_to_print, my_strdup(data));

    /* Customers info */
    sprintf(data, "Clientes:\n"); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Ficheiro: %s\n", q->customersPath); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Clientes válidos: %d\n", q->validCustomers); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Clientes inválidos: %d\n", (q->totalCustomers - q->validCustomers)); g_ptr_array_add(q->array_to_print, my_strdup(data));
    sprintf(data, "Total de clientes: %d\n", q->totalCustomers); g_ptr_array_add(q->array_to_print, my_strdup(data));

    free(data);

    return q->array_to_print;
}

void free_query13(void* ptr){
    Query13 q = (Query13) ptr;

    free(q->customersPath);
    free(q->productsPath);
    free(q->salesPath);
    g_ptr_array_free(q->array_to_print, TRUE);

    free(q);
}

PrintableResults getCurrentFilesInfo(SGV sgv){
    Query13 q = malloc(sizeof(struct query13));

    /* Copy paths */
    q->salesPath = my_strdup(sgv->salesPath);
    q->productsPath = my_strdup(sgv->productsPath);
    q->customersPath = my_strdup(sgv->customersPath);

    /* Copy info */
    q->validSales = sgv->validSales;
    q->totalSales = sgv->totalSales;
    q->validProducts = getValidProducts(sgv->products);
    q->totalProducts = getTotalProducts(sgv->products);
    q->validCustomers = getValidCustomers(sgv->customers);
    q->totalCustomers = getTotalCustomers(sgv->customers);

    PrintableResults pr = initPrintableResults(q, print_query13, free_query13, 0);

    return pr;
}
