#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "interface.h"

#define LINE_SIZE_PRODS    6
#define LINE_SIZE_CLIENTES 5
#define LINE_SIZE_VENDAS   40

struct sgv{
    CatProds products;
    CatClientes customers;
    Faturacao f;
    Filial* branches;
    Stats stats;

    int validSales, totalSales;
    char *salesPath, *productsPath, *customersPath;

    int isLoaded; /* 1 = Yes */
    int timingModeOn; /* 1 = Yes */
    int saveQueries; /* 1 = Yes */
};

SGV initSGV(){
    int i;
    SGV sgv = malloc(sizeof(struct sgv));
    sgv->products = initCatProds();
    sgv->customers = initCatClientes();
    sgv->f = init_faturacao();
    sgv->branches = malloc(sizeof(Filial) * get_num_branches());
    for(i = 0; i < get_num_branches(); i++) sgv->branches[i] = init_filial();
    sgv->isLoaded = 0;
    sgv->timingModeOn = 0;
    sgv->saveQueries = 0;

    return sgv;
}

void setTimingModeOn(SGV sgv){
    sgv->timingModeOn = 1;
    char *stats_path = get_stats_path(), *results_path = get_results_path();
    sgv->stats = init_stats(stats_path, results_path, 0, 0);
    free(stats_path); free(results_path);
}

void destroySGV(SGV sgv){
    if(!sgv->isLoaded) return;

    int i;
    destroyCatProds(sgv->products);
    destroyCatClientes(sgv->customers);
    destroy_faturacao(sgv->f);
    for(i = 0; i < get_num_branches(); i++) destroy_filial(sgv->branches[i]);
    free(sgv->branches);
    if(sgv->stats) destroy_stats(sgv->stats);

    free(sgv->salesPath);
    free(sgv->productsPath);
    free(sgv->customersPath);
    free(sgv);
}

SGV loadSGVFromFiles(SGV sgv, char* customersPath, char* productsPath, char* salesPath){
    if(sgv->isLoaded) return NULL;

    sgv->productsPath = my_strdup(productsPath); loadCatalog(sgv, productsPath, 0);
    sgv->customersPath = my_strdup(customersPath); loadCatalog(sgv, customersPath, 1);
    sgv->salesPath = my_strdup(salesPath); loadVendas(sgv, salesPath, LINE_SIZE_VENDAS);
    sgv->isLoaded = 1;

    return sgv;
}

/* ----------------------- SGV INTERFACE ----------------------------- */
int isLoaded(SGV sgv){
    return sgv->isLoaded;
}

int isTimingModeOn(SGV sgv){
    return sgv->timingModeOn;
}

void turnOnQuerySavingMode(SGV sgv){
    sgv->saveQueries = 1;
}

int isQuerySavingModeOn(SGV sgv){
    return sgv->saveQueries;
}

int getNumberOfValidProducts(SGV sgv){
    return getValidProducts(sgv->products);
}

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
    for(i = 0; i < get_num_branches(); i++) addEmptyCustomerToBranch(sgv->branches[i], customer);
}

void sgv_customerBought(SGV sgv, Venda v){
    int branch = getBranch(v);
    customerBought(sgv->branches[branch-1], v);
}

GPtrArray* sgv_productInfoAsString(SGV sgv, char* product){
    return productInfoAsString(sgv->f, product);
}

void sgv_start_timing(SGV sgv, char* name){
    if(sgv->timingModeOn) start_timing(sgv->stats, name);
}

void sgv_agreggate_timing(SGV sgv, char* name){
    if(sgv->timingModeOn) agreggate_timing(sgv->stats, name);
}

void sgv_finish_timing(SGV sgv, char* name){
    if(sgv->timingModeOn) finish_timing(sgv->stats, name);
}

/* --------------------------- QUERIES ------------------------------- */

/* --------------------------- QUERY 2 ------------------------------- */
gboolean end_with_newline(gpointer data, gpointer user_data){
    GPtrArray* array = (GPtrArray*) user_data;
    char* product = (char*) data;
    char* string = malloc(sizeof(char) * 20);
    sprintf(string, "%s\n", product);

    g_ptr_array_add(array, my_strdup(string));
    free(string);

    return FALSE;
}

GPtrArray* getProductsStartedByLetter(SGV sgv, char letter){
    sgv_start_timing(sgv, "query2");

    GPtrArray* products = retrieve_letter_products(sgv->products, letter);

    sgv_finish_timing(sgv, "query2");

    return products;
}

/* --------------------------- QUERY 3 ------------------------------- */
void free_query3(void* ptr){
    Query3 q = (Query3) ptr;
    free(q->product);
    free(q);
}

Query3 getProductSalesAndProfitInBranch(SGV sgv, char* productID, int month, int branch){
    Query3 q = malloc(sizeof(struct query3));

    q->product = my_strdup(productID);
    q->month = month;
    q->vendas_N = getProductMonthlySalesInBranch(sgv->f, productID, 'N', branch, month);
    q->profit_N = getProductMonthlyProfitInBranch(sgv->f, productID, 'N', branch, month);
    q->vendas_P = getProductMonthlySalesInBranch(sgv->f, productID, 'P', branch, month);
    q->profit_P = getProductMonthlyProfitInBranch(sgv->f, productID, 'P', branch, month);

    return q;
}

 /*Agreggate tells us if we want the result branch by branch or gathered*/
Query3* getProductSalesAndProfit(SGV sgv, char* productID, int month, int agreggate){
    int i;
    Query3 *ret;

    sgv_start_timing(sgv, "query3");

    Query3* branchs = malloc(sizeof(struct query3) * (get_num_branches()+1));
    for(i = 0; i < get_num_branches(); i++){
        branchs[i] = getProductSalesAndProfitInBranch(sgv, productID, month, i+1);
    }
    branchs[i] = 0;
    Query3 total = malloc(sizeof(struct query3));
    total->product = my_strdup(productID); total->month = month;
    total->vendas_N = 0; total->vendas_P = 0; total->profit_N = 0; total->profit_P = 0;
    for(i = 0; i < get_num_branches(); i++){
        total->vendas_N += branchs[i]->vendas_N;
        total->vendas_P += branchs[i]->vendas_P;
        total->profit_N += branchs[i]->profit_N;
        total->profit_P += branchs[i]->profit_P;
    }

    if(agreggate){
        for(i = 0; i < get_num_branches(); i++) free(branchs[i]);
        free(branchs);
        ret = malloc(sizeof(struct query3) * 2);
        *ret = total; ret[1] = 0;
    } else {
        free(total);
        ret = branchs;
    }

    sgv_finish_timing(sgv, "query3");

    return ret;
}

/* --------------------------- QUERY 4 ------------------------------- */
struct query4{
    GPtrArray* array;
    Faturacao f;
    int branchToCheck;
};

typedef struct query4 *Query4;

gboolean insert_in_array2(gpointer data, gpointer user_data){
    Query4 q = (Query4) user_data;
    char* string = malloc(sizeof(char) * 20);
    sprintf(string, "%s", (char*) data);

    if(q->branchToCheck && wasSoldInBranch(q->f, (char*) data, q->branchToCheck)) g_ptr_array_add(q->array, my_strdup(string));
    else if(wasSold(q->f, (char*) data)) g_ptr_array_add(q->array, my_strdup(string));

    free(string);

    return FALSE;
}

GPtrArray** getProductsNeverBought(SGV sgv, int oneByOne){
    int i;
    GPtrArray** ret;

    sgv_start_timing(sgv, "query4");
    GPtrArray* products = allProducts(sgv->products);

    if(oneByOne){
        Query4* oneByOneQuery = malloc(sizeof(Query4) * get_num_branches());
        for(i = 0; i < get_num_branches(); i++) {
            oneByOneQuery[i] = malloc(sizeof(struct query4));
            oneByOneQuery[i]->array = g_ptr_array_new_with_free_func(free);
            oneByOneQuery[i]->f = sgv->f;
            oneByOneQuery[i]->branchToCheck = i+1;
        }

        for(i = 0; i < get_num_branches(); i++) g_ptr_array_foreach(products, (GFunc) insert_in_array2, oneByOneQuery[i]);

        ret = malloc(sizeof(GPtrArray*) * (get_num_branches()+1));
        for(i = 0; i < get_num_branches(); i++) { ret[i] = oneByOneQuery[i]->array; free(oneByOneQuery[i]); }
        ret[i] = 0; free(oneByOneQuery);
    } else {
        Query4 q = malloc(sizeof(struct query4));
        q->array = g_ptr_array_new_with_free_func(free); q->f = sgv->f; q->branchToCheck = 0;

        g_ptr_array_foreach(products, (GFunc) insert_in_array2, q);

        ret = malloc(sizeof(GPtrArray*) * 2);
        *ret = q->array; ret[1] = 0;
        free(q);
    }
    sgv_finish_timing(sgv, "query4");
    g_ptr_array_free(products, TRUE);

    return ret;
}

/* --------------------------- QUERY 5 ------------------------------- */
struct query5{
    GPtrArray* array;
    SGV sgv;
};

typedef struct query5 *Query5;

void insert_in_array_query5(gpointer data, gpointer user_data){
    Query5 q = (Query5) user_data;
    char* customer = (char*) data;
    int i, bought = 0;

    for(i = 0; i < get_num_branches(); i++) bought += didCustomerBought(q->sgv->branches[i], customer);

    char* string = malloc(sizeof(char) * 10);
    sprintf(string, "%s", customer);
    if(!bought) g_ptr_array_add(q->array, my_strdup(string));

    free(string);
}

GPtrArray* getClientsOfAllBranches(SGV sgv){
    GPtrArray* customers = allCustomers(sgv->customers);
    GPtrArray* ret = g_ptr_array_new_with_free_func(free);

    Query5 q = malloc(sizeof(struct query5)); q->array = ret; q->sgv = sgv;

    sgv_start_timing(sgv, "query5");
    g_ptr_array_foreach(customers, (GFunc) insert_in_array_query5, q);
    sgv_finish_timing(sgv, "query5");

    free(q);
    g_ptr_array_free(customers, TRUE);

    return ret;
}

/* --------------------------- QUERY 6 ------------------------------- */
struct query6{
    int productsNeverBought;
    int customersThatNeverBought;
    SGV sgv;
};

typedef struct query6 *Query6;

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

    for(i = 0; i < get_num_branches(); i++) bought += didCustomerBought(q->sgv->branches[i], customer);
    if(bought) q->customersThatNeverBought++;

    return FALSE;
}

int* getClientsAndProductsNeverBoughtCount(SGV sgv){
    int* ret = malloc(sizeof(int) * 2);

    Query6 q = malloc(sizeof(struct query6));
    q->productsNeverBought = 0;
    q->customersThatNeverBought = 0;
    q->sgv = sgv;

    sgv_start_timing(sgv, "query6");
    GPtrArray* products = allProducts(sgv->products);
    GPtrArray* customers = allCustomers(sgv->customers);

    g_ptr_array_foreach(products, (GFunc) countNotSold, q);
    g_ptr_array_foreach(customers, (GFunc) countDidntBuy, q);
    sgv_finish_timing(sgv, "query6");

    g_ptr_array_free(products, TRUE);
    g_ptr_array_free(customers, TRUE);

    ret[0] = q->productsNeverBought; ret[1] = q->customersThatNeverBought;
    free(q);

    return ret;
}

/* --------------------------- QUERY 7 ------------------------------- */
int** getProductsBoughtByClient(SGV sgv, char* clientID){
    int** monthlyAmounts = malloc(sizeof(int*) * (get_num_branches() + 1));
    int i, j;

    sgv_start_timing(sgv, "query7");

    for(i = 0; i < get_num_branches(); i++){
        monthlyAmounts[i] = malloc(sizeof(int) * (get_num_meses() + 1));
        for(j = 0; j < get_num_meses(); j++) monthlyAmounts[i][j] = 0;
        monthlyAmounts[i][j] = -1;
        getProductsBoughtByClientInBranch(sgv->branches[i], monthlyAmounts[i], clientID);
    }
    monthlyAmounts[i] = 0;

    sgv_finish_timing(sgv, "query7");

    return monthlyAmounts;
}

/* --------------------------- QUERY 8 ------------------------------- */
Pair getSalesAndProfit(SGV sgv, int minMonth, int maxMonth){
    int i;
    long *total_vendas = malloc(sizeof(long)); *total_vendas = 0;
    float *total_faturado = malloc(sizeof(float)); *total_faturado = 0;

    sgv_start_timing(sgv, "query8");

    for(i = minMonth; i <= maxMonth; i++){
        *total_faturado += getMonthlyProfit(sgv->f, i);
        *total_vendas += getMonthlySales(sgv->f, i);
    }

    sgv_finish_timing(sgv, "query8");

    Pair p = mkPairWithFreeFunc(total_vendas, free, total_faturado, free);

    return p;
}
/* --------------------------- QUERY 9 ------------------------------- */
Pair getProductBuyers(SGV sgv, char* productID, int branch){
    sgv_start_timing(sgv, "query9");

    GPtrArray** arrays = getProductBuyersInBranch(sgv->branches[branch-1], productID);
    /* arrays[0] == regular && arrays[1] == promotion */
    Pair p = mkPairWithFreeFunc(arrays[0], free_gptr_array, arrays[1], free_gptr_array);
    free(arrays);

    sgv_finish_timing(sgv, "query9");

    return p;
}

/* --------------------------- QUERY 10 ------------------------------ */
gboolean tableToHeap(gpointer key, gpointer value, gpointer data){
    PQueue* heap = (PQueue*) data;
    char* product = my_strdup((char*) key);
    int* productAmount = malloc(sizeof(int)); *productAmount = *((int*) value);

    pqueue_enqueue(heap, productAmount, product);

    return FALSE;
}

PQueue* getClientsFavouriteProducts(SGV sgv, char* clientID, int month){
    int i;
    PQueue* min_heap = pqueue_new(compare_ints_two_args, free, free, 1000000);
    GHashTable* table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    sgv_start_timing(sgv, "query10");
    /* Fully gather the table */
    for(i = 0; i < get_num_branches(); i++) {
        getClientFavProducts(sgv->branches[i], clientID, month, table);
    }

    /* Turn it into a heap */
    g_hash_table_foreach(table, (GHFunc) tableToHeap, min_heap);
    g_hash_table_destroy(table);
    sgv_finish_timing(sgv, "query10");

    return min_heap;
}

/* --------------------------- QUERY 11 ------------------------------ */
void recursive_depth1_free(void* f){
    void** ptr = (void**) f;
    int i;
    for(i = 0; ptr[i]; i++) free(ptr[i]);
    free(ptr);
}

gboolean tableToHeap_Query11(gpointer key, gpointer value, gpointer data){
    PQueue* heap = (PQueue*) data;
    char* product = my_strdup((char*) key);
    int **array = ((int**) value);

    /* Clone the value we have in the table */
    int **product_info_clone = malloc(sizeof(int*) * (get_num_branches() + 2)), i;
    for(i = 0; i < get_num_branches() + 1; i++){
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
    Pair p = mkPairWithFreeFunc(product, free, product_info_clone, recursive_depth1_free);

    /* We use the amountPurchased as key (that's the sorting criteria) */
    pqueue_enqueue(heap, amountPurchased, p);

    return FALSE;
}

GPtrArray* getTopSelledProducts(SGV sgv, int limit){
    int i;
    PQueue* max_heap = pqueue_new(compare_ints_two_args, free, freePair, 1000000);
    GHashTable* hashtable = g_hash_table_new_full(g_str_hash, g_str_equal, free, recursive_depth1_free);
    int* info = malloc(sizeof(int) * 2); /* index 0 is num_branches, index 1 is current */
    info[0] = get_num_branches();

    sgv_start_timing(sgv, "query11");

    /* Pass the empty array directly into the branchs to fill it up */
    for(i = 0; i < get_num_branches(); i++){
        info[1] = i+1;
        char* string = malloc(sizeof(char) * 100);
        sprintf(string, "query11_loadTree%d", i+1);
        sgv_start_timing(sgv, string);

        getSoldProductsInBranch(sgv->branches[i], hashtable, info);

        sgv_finish_timing(sgv, string);
        free(string);
    }

    /* Turn it into a max-heap */
    sgv_start_timing(sgv, "query11_treeToHeap");
    g_hash_table_foreach(hashtable, (GHFunc) tableToHeap_Query11, max_heap);
    sgv_finish_timing(sgv, "query11_treeToHeap");

    g_hash_table_destroy(hashtable);
    free(info);
    sgv_finish_timing(sgv, "query11");

    /* Extract N = limit elements into an array */
    GPtrArray* ret = g_ptr_array_new_with_free_func(freePair);
    for(i = 0; i < limit; i++) {
        Pair p = (Pair) pqueue_dequeue(max_heap);
        g_ptr_array_add(ret, p);
    }

    /* Destroy the heap */
    pqueue_delete(max_heap);

    return ret;
}

/* --------------------------- QUERY 12 ------------------------------ */
gboolean tableToHeap_Query12(gpointer key, gpointer value, gpointer data){
    PQueue* queue = (PQueue*) data;
    char* product = my_strdup((char*) key);

    /* Do two copies so we can have it stored in the value too */
    float *amount_bought_key = malloc(sizeof(float)); *amount_bought_key = *((float*) value);
    float *amount_bought_value = malloc(sizeof(float)); *amount_bought_value = *((float*) value);

    /* Setup the pair to go into the value field */
    Pair p = mkPairWithFreeFunc(product, free, amount_bought_value, free);

    /* We use the amountPurchased as key (that's the sorting criteria) */
    pqueue_enqueue(queue, amount_bought_key, p);

    return FALSE;
}

GPtrArray* getClientTopProfitProducts(SGV sgv, char* clientID, int limit){
    int i;
    PQueue* queue = pqueue_new(compare_floats_two_args, free, freePair, 1000000);
    GHashTable* table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    sgv_start_timing(sgv, "query12");
    /* Fully gather the table */
    for(i = 0; i < get_num_branches(); i++) {
        getClientTopProfitProductsBranch(sgv->branches[i], clientID, table);
    }

    /* Turn it into a heap */
    g_hash_table_foreach(table, (GHFunc) tableToHeap_Query12, queue);
    g_hash_table_destroy(table);
    sgv_finish_timing(sgv, "query12");

    /* Extract N = limit elements into an array */
    GPtrArray* ret = g_ptr_array_new_with_free_func(freePair);
    for(i = 0; i < limit; i++) {
        Pair p = (Pair) pqueue_dequeue(queue);
        if(!p) break;
        g_ptr_array_add(ret, p);
    }

    /* Destroy the queue */
    pqueue_delete(queue);

    return ret;
}

/* --------------------------- QUERY 13 ------------------------------ */
Query13 getCurrentFilesInfo(SGV sgv){
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

    return q;
}
