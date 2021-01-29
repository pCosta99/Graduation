#ifndef __SGV__
#define __SGV__

#include "faturacao.h"
#include "printableResults.h"
#include "stats.h"
#include "catprods.h"
#include "catclientes.h"
#include "filial.h"

 /* Opaque data type defined in this module */
typedef struct sgv* SGV;

/* Inits the SGV */
SGV initSGV();

/* Inits the SGV with timing mode on */
SGV initSGVWithTimingOn();

/* Destroys the SGV */
void destroySGV(SGV sgv);

/* Loads SGV directly from specified files */
SGV loadSGVFromFiles(SGV sgv, char* customersPath, char* productsPath, char* salesPath);

/* Loads SGV from the folder indicated with the default files inside  */
SGV loadSGVFromFolder(SGV sgv, char* filesFolderPath);

/* ----------------------- SGV INTERFACE ----------------------------- */
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

Stats getStats(SGV sgv);

/* --------------------------- QUERIES ------------------------------- */

/* Query 2 */
PrintableResults getProductsStartedByLetter(SGV sgv, char letter);

/* Query 3 */
PrintableResults getProductSalesAndProfit(SGV sgv, char* productID, int month, int agreggate);

/* Query 4 */
PrintableResults getProductsNeverBought(SGV sgv, int oneByOne);

/* Query 5 */
PrintableResults getClientsOfAllBranches(SGV sgv);

/* Query 6 */
PrintableResults getClientsAndProductsNeverBoughtCount(SGV sgv);

/* Query 7 */
PrintableResults getProductsBoughtByClient(SGV sgv, char* clientID);

/* Query 8 */
PrintableResults getSalesAndProfit(SGV sgv, int minMonth, int maxMonth);

/* Query 9 */
PrintableResults getProductBuyers(SGV sgv, char* productID, int branch);

/* Query 10 */
PrintableResults getClientsFavouriteProducts(SGV sgv, char* clientID, int month);

/* Query 11 */
PrintableResults getTopSelledProducts(SGV sgv, int limit);

/* Query 12 */
PrintableResults getClientTopProfitProducts(SGV sgv, char* clientID, int limit);

/* Query 13 */
PrintableResults getCurrentFilesInfo(SGV sgv);

#endif
