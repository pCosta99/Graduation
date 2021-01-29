#ifndef __SGV__
#define __SGV__

#include "faturacao.h"
#include "stats.h"
#include "catprods.h"
#include "catclientes.h"
#include "filial.h"
#include "pqueue.h"
#include "pair.h"
#include "config.h"

 /* Opaque data type defined in this module */
typedef struct sgv* SGV;

/* Inits the SGV */
SGV initSGV();

/* Sets the SGV timing mode on */
void setTimingModeOn(SGV sgv);

/* Destroys the SGV */
void destroySGV(SGV sgv);

/* Loads SGV directly from specified files */
SGV loadSGVFromFiles(SGV sgv, char* customersPath, char* productsPath, char* salesPath);

/* ----------------------- SGV INTERFACE ----------------------------- */
/* Checks if SGV is loaded. 1 means yes */
int isLoaded(SGV sgv);

/* Checks if SGV timing mode in on. 1 means yes */
int isTimingModeOn(SGV sgv);

/* Turns query saving mode on */
void turnOnQuerySavingMode(SGV sgv);

/* Tells us if query saving mode is on. 1 means yes. */
int isQuerySavingModeOn(SGV sgv);

/* Returns the amount of valid products */
int getNumberOfValidProducts(SGV sgv);

/* Sets the amount of valid sales */
void setValidSales(SGV sgv, int validSales);

/* Sets the total number of sales */
void setTotalSales(SGV sgv, int totalSales);

/* Sets the amount of valid products */
void sgv_setValidProducts(SGV sgv, int validProducts);

/* Sets the total number of products */
void sgv_setTotalProducts(SGV sgv, int totalProducts);

/* Sets the amount of valid customers */
void sgv_setValidCustomers(SGV sgv, int validCustomers);

/* Sets the number of total customers */
void sgv_setTotalCustomers(SGV sgv, int totalCustomers);

/* Adds a product to the corresponding catalog */
void sgv_addProductToCatalog(SGV sgv, char* product);

/* Checks if a product exists. Returns 0 if it does, 1 otherwise */
int sgv_checkIfProductExists(SGV sgv, char* product);

/* Adds a customer to the corresponding catalog */
void sgv_addCustomerToCatalog(SGV sgv, char* customer);

/* Checks if a customer exists. Returns 0 if it does, 1 otherwise */
int sgv_checkIfCustomerExists(SGV sgv, char* customer);

/* Adds an empty product to Faturacao */
void sgv_addEmptyProductToFac(SGV sgv, char* product);

/* Bills a product */
void sgv_billProduct(SGV sgv, Venda v);

/*Adds an empty customer to the corresponding branch*/
void sgv_addEmptyCustomerToBranches(SGV sgv, char* customer);

/* Bills a product */
void sgv_customerBought(SGV sgv, Venda v);

/* Returns info about a product */
GPtrArray* sgv_productInfoAsString(SGV sgv, char* product);

/* Starts timing */
void sgv_start_timing(SGV sgv, char* name);

/* Agreggates the time passed */
void sgv_agreggate_timing(SGV sgv, char* name);

/* Finishes timing */
void sgv_finish_timing(SGV sgv, char* name);

/* --------------------------- QUERIES ------------------------------- */

/* --------------------------- QUERY 2 ------------------------------- */
GPtrArray* getProductsStartedByLetter(SGV sgv, char letter);

/* --------------------------- QUERY 3 ------------------------------- */
struct query3{
    int month; char* product;
    int vendas_N; int vendas_P;
    float profit_N; float profit_P;
};

typedef struct query3 *Query3;

Query3* getProductSalesAndProfit(SGV sgv, char* productID, int month, int agreggate);

/* --------------------------- QUERY 4 ------------------------------- */
GPtrArray** getProductsNeverBought(SGV sgv, int oneByOne);

/* --------------------------- QUERY 5 ------------------------------- */
GPtrArray* getClientsOfAllBranches(SGV sgv);

/* --------------------------- QUERY 6 ------------------------------- */
int* getClientsAndProductsNeverBoughtCount(SGV sgv);

/* --------------------------- QUERY 7 ------------------------------- */
int** getProductsBoughtByClient(SGV sgv, char* clientID);

/* --------------------------- QUERY 8 ------------------------------- */
/* (x,y) --> x == sales && y == profit */
Pair getSalesAndProfit(SGV sgv, int minMonth, int maxMonth);

/* --------------------------- QUERY 9 ------------------------------- */
/* (x,y) --> x == regular && y == promotion */
Pair getProductBuyers(SGV sgv, char* productID, int branch);

/* --------------------------- QUERY 10 ------------------------------- */
PQueue* getClientsFavouriteProducts(SGV sgv, char* clientID, int month);

/* --------------------------- QUERY 11 ------------------------------- */
/* Array of pairs: (x,y) --> x == product && y == product_info */
GPtrArray* getTopSelledProducts(SGV sgv, int limit);

/* --------------------------- QUERY 12 ------------------------------- */
/* Array of pairs: (x,y) --> x == product && y == amount_paid */
GPtrArray* getClientTopProfitProducts(SGV sgv, char* clientID, int limit);

/* --------------------------- QUERY 13 ------------------------------- */
struct query13{
    char* salesPath;
    int validSales, totalSales;
    char* customersPath;
    int validCustomers, totalCustomers;
    char* productsPath;
    int validProducts, totalProducts;
};

typedef struct query13 *Query13;

Query13 getCurrentFilesInfo(SGV sgv);

#endif
