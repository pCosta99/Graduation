#include "pair.h"
#include "catalog.h"
#include "filial.h"

struct customerbought{
    char* product;
    int** n_compras;
    float** total_gasto;
};

typedef struct customerbought *CustomerBought;

void accumulateSaleOnCustomerBoughtInfo(CustomerBought cb, Venda v){
    int amount = getAmount(v);
    float profit = getPrice(v) * amount;
    int month = getMonth(v);
    int index = getType(v) == 'P';

    cb->n_compras[index][month-1] += amount;
    cb->total_gasto[index][month-1] += profit;
}

CustomerBought createCustomerBought(char* product, Venda v){
    int i, j;
    CustomerBought cb = malloc(sizeof(struct customerbought));

    cb->product = my_strdup(product);
    cb->n_compras = malloc(sizeof(int*) * get_num_types());
    cb->total_gasto = malloc(sizeof(float*) * get_num_types());
    for(i = 0; i < get_num_types(); i++){
        cb->n_compras[i] = malloc(sizeof(int) * get_num_meses());
        cb->total_gasto[i] = malloc(sizeof(float) * get_num_meses());
        for(j = 0; j < get_num_meses(); j++){
            cb->n_compras[i][j] = 0;
            cb->total_gasto[i][j] = 0;
        }
    }

    accumulateSaleOnCustomerBoughtInfo(cb, v);

    return cb;
}

void free_customerbought(void* ptr){
    int i;
    CustomerBought cb = (CustomerBought) ptr;
    free(cb->product);
    for(i = 0; i < get_num_types(); i++){
        free(cb->n_compras[i]);
        free(cb->total_gasto[i]);
    }
    free(cb->n_compras);
    free(cb->total_gasto);
    free(cb);
}

int getNCompras(CustomerBought cb, char type){
    int total = 0, i;
    for(i = 0; i < get_num_meses(); i++) total += cb->n_compras[type=='P'][i];
    return total;
}

int getNComprasMes(CustomerBought cb, char type, int month){
    return cb->n_compras[type=='P'][month-1];
}

float getAllProfit(CustomerBought cb, char type){
    float total = 0; int i;
    for(i = 0; i < get_num_meses(); i++) total += cb->total_gasto[type=='P'][i];
    return total;
}

struct customerinfo{
    /* key -> char* customer, value -> CustomerBought cb */
    GHashTable* customerBought;
    char* customer;
    int numberOfBuys;
};

typedef struct customerinfo *CustomerInfo;

CustomerInfo createCustomerInfo(char* customer){
    CustomerInfo ci = malloc(sizeof(struct customerinfo));

    ci->customerBought = g_hash_table_new_full(g_str_hash, g_str_equal, free, free_customerbought);
    ci->customer = my_strdup(customer);
    ci->numberOfBuys = 0;

    return ci;
}

void destroy_customerinfo(CustomerInfo ci){
    g_hash_table_destroy(ci->customerBought);
    free(ci->customer);
    free(ci);
}

void accumulateSaleOnCustomerInfo(CustomerInfo ci, Venda v){
    char* product = getProduct(v);
    CustomerBought cb = NULL;
    ci->numberOfBuys++;

    cb = g_hash_table_lookup(ci->customerBought, product);
    if(cb) {
        accumulateSaleOnCustomerBoughtInfo(cb, v);
    }
    else {
        cb = createCustomerBought(product, v);
        char* dup_product = my_strdup(product);
        g_hash_table_insert(ci->customerBought, dup_product, cb);
    }
    free(product);
}

struct filial{
    Catalog branchCustomers;
};

/* Initializes the structure */
Filial init_filial(){
    Filial f = malloc(sizeof(struct filial));

    f->branchCustomers = init_catalog(get_num_letters(), get_customers_depth(), compare_ints, free, destroy_customerinfo);

    return f;
}

void destroy_filial(Filial f){
    destroy_catalog(f->branchCustomers);
    free(f);
}

void addEmptyCustomerToBranch(Filial f, char* customer){
    CustomerInfo ci = createCustomerInfo(customer);
    catalog_insert(f->branchCustomers, customer, ci);
}

void customerBought(Filial f, Venda v){
    char* customer = getCustomer(v);

    CustomerInfo ci = catalog_get(f->branchCustomers, customer);
    accumulateSaleOnCustomerInfo(ci, v);

    free(customer);
}

int didCustomerBought(Filial f, char* customer){
    CustomerInfo ci = catalog_get(f->branchCustomers, customer);
    return !ci->numberOfBuys;
}

gboolean func(gpointer key, gpointer value, gpointer data){
    CustomerBought cb = (CustomerBought) value;
    int* array = (int*) data;
    int i;

    for(i = 0; i < get_num_meses(); i++) array[i] += (getNComprasMes(cb, 'P', i+1) + getNComprasMes(cb, 'N', i+1));

    return FALSE;
}

void getProductsBoughtByClientInBranch(Filial f, int* monthlyAmounts, char* customer){
    CustomerInfo ci = catalog_get(f->branchCustomers, customer);

    g_hash_table_foreach(ci->customerBought, (GHFunc) func, monthlyAmounts);
}

gboolean checkProductSold(gpointer key, gpointer value, gpointer data){
    CustomerInfo ci = (CustomerInfo) value;
    Pair p = (Pair) data;
    GPtrArray** arrays = (GPtrArray**) fst(p);

    CustomerBought cb = (CustomerBought) g_hash_table_lookup(ci->customerBought, (char*) snd(p));
    if(cb && getNCompras(cb, 'N')) { g_ptr_array_add(arrays[0], my_strdup(ci->customer)); }
    if(cb && getNCompras(cb, 'P')) { g_ptr_array_add(arrays[1], my_strdup(ci->customer)); }

    return FALSE;
}

GPtrArray** getProductBuyersInBranch(Filial f, char* product){
    GPtrArray** ret = malloc(sizeof(GPtrArray*) * 2);
    ret[0] = g_ptr_array_new_with_free_func(free);
    ret[1] = g_ptr_array_new_with_free_func(free);
    Pair p = mkPair(ret, product);

    catalog_foreach(f->branchCustomers, (GTraverseFunc) checkProductSold, p);

    freePair(p);

    return ret;
}

gboolean getAmountSoldInMonth(gpointer key, gpointer value, gpointer data){
    Pair p = (Pair) data;
    int month = *((int*) snd(p));
    CustomerBought cb = (CustomerBought) value;

    int total = getNComprasMes(cb, 'P', month) + getNComprasMes(cb, 'N', month);

    if(total) {
        char* product = (char*) key;
        GHashTable* table = (GHashTable*) fst(p);
        int* amount = g_hash_table_lookup(table, product);
        if(amount){
            *amount += total;
        } else {
            int* perm_total = malloc(sizeof(int)); *perm_total = total;
            g_hash_table_insert(table, my_strdup(product), perm_total);
        }
    }

    return FALSE;
}

void getClientFavProducts(Filial f, char* customer, int month, GHashTable* table){
    int* month_ptr = malloc(sizeof(int)); *month_ptr = month;
    Pair p = mkPair(table, month_ptr);

    CustomerInfo ci = catalog_get(f->branchCustomers, customer);
    g_hash_table_foreach(ci->customerBought, (GHFunc) getAmountSoldInMonth, p);
    free(month_ptr);
    freePair(p);
}

gboolean traverse_CustomerBought(gpointer key, gpointer value, gpointer data){
    int i;
    /* Cast everything into it's correct types */
    char* product = (char*) key;
    CustomerBought cb = (CustomerBought) value;
    Pair p = (Pair) data;
    GHashTable* table = (GHashTable*) fst(p);
    int number_branches = ((int*) snd(p))[0];
    int current_branch = ((int*) snd(p))[1]; /* the index 0 is reserved for total values */
    int** table_product_info = g_hash_table_lookup(table, product); /* Look for the product in the table */

    /* If it did not exist, we create the info container */
    if(table_product_info == NULL) {
        table_product_info = malloc(sizeof(int*) * (number_branches + 2));
        for(i = 0; i < number_branches + 1; i++) {
            table_product_info[i] = malloc(sizeof(int) * 2);
            table_product_info[i][0] = 0;
            table_product_info[i][1] = 0;
        }
        table_product_info[i] = 0;
        g_hash_table_insert(table, my_strdup(product), table_product_info);
    }

    /* At this point, the entries exist. Therefore, we accumulate the new values */
    int sales = getNCompras(cb, 'P') + getNCompras(cb, 'N');
    table_product_info[0][0]++;
    table_product_info[0][1] += sales;
    table_product_info[current_branch][0]++; /* It has one more customer */
    table_product_info[current_branch][1] += sales; /* It was sold those times */

    return FALSE;
}

gboolean traverse_CustomerInfo(gpointer key, gpointer value, gpointer data){
    /* We first get the info about a customer (because we want the map inside) */
    CustomerInfo ci = (CustomerInfo) value;

    /* Then we traverse the product map to create a new one with the relevant information */
    g_hash_table_foreach(ci->customerBought, (GHFunc) traverse_CustomerBought, data);

    return FALSE;
}

void getSoldProductsInBranch(Filial f, GHashTable* table, int* numAndCurrentBranch){
    Pair p = mkPair(table, numAndCurrentBranch);
    catalog_foreach(f->branchCustomers, (GTraverseFunc) traverse_CustomerInfo, p);
    freePair(p);
}

gboolean profitProductsBranch(gpointer key, gpointer value, gpointer data){
    char* product = (char*) key;
    CustomerBought cb = (CustomerBought) value;
    GHashTable* table = (GHashTable*) data;

    float* amount_spent = g_hash_table_lookup(table, product);

    /* If it did not exist, we create the info container */
    if(amount_spent == NULL){
        amount_spent = malloc(sizeof(float));
        *amount_spent = 0;
        g_hash_table_insert(table, my_strdup(product), amount_spent);
    }

    /* Accumulate the info of this product on the container created above */
    *amount_spent = getAllProfit(cb, 'P') + getAllProfit(cb, 'N');

    return FALSE;
}

void getClientTopProfitProductsBranch(Filial f, char* clientID, GHashTable *table){
    /* Retrieve the customer info we are interested in */
    CustomerInfo ci = (CustomerInfo) catalog_get(f->branchCustomers, clientID);

    /* Go through the customer products and add them all into the table */
    g_hash_table_foreach(ci->customerBought, (GHFunc) profitProductsBranch, table);
}

/*
int main(void){
    Filial f = init_filial();

    Venda v = createSale("AF1184", 1, 1, 'N', "A1234", 1, 1);
    customerBought(f, v);
    v = createSale("AF1184", 1, 1, 'P', "A1234", 1, 1);
    customerBought(f, v);



    destroy_filial(f);
}*/
