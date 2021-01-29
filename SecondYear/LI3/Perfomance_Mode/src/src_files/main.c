#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgv.h"

#define PATH_TO_FILES      "Files"
#define PATH_TO_RESULTS    "Stats/Results"

void printToFile(char* queryName){
    char* path = malloc(sizeof(char) * 100);
    sprintf(path, "%s/%s", PATH_TO_RESULTS, queryName);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fd, 1);
    free(path);
}

/* Query 2 -> Correto (ATENÇÃO: Não se pode dar free a memoria aqui) */
/* Verified */
void query2(SGV sgv, char letter){
    fflush(stdout);
    PrintableResults pr = getProductsStartedByLetter(sgv, letter);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query2");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 3 (Forma 1) -> Correta */
/* Verified */
void query3_f1(SGV sgv, char* product, int month, int agreggate){
    fflush(stdout);
    PrintableResults pr = getProductSalesAndProfit(sgv, product, month, agreggate);
    GPtrArray** array = toMultipleArrayOfStrings(pr);
    printToFile("query3_B1");
    g_ptr_array_foreach(array[0], (GFunc) printf, NULL);
    fflush(stdout);
    printToFile("query3_B2");
    g_ptr_array_foreach(array[1], (GFunc) printf, NULL);
    fflush(stdout);
    printToFile("query3_B3");
    g_ptr_array_foreach(array[2], (GFunc) printf, NULL);
    g_ptr_array_set_free_func(array[0], free);
    g_ptr_array_free (array[0], TRUE);
    g_ptr_array_set_free_func(array[1], free);
    g_ptr_array_free (array[1], TRUE);
    g_ptr_array_set_free_func(array[2], free);
    g_ptr_array_free (array[2], TRUE);
    free(array);
    destroy_pr(pr);
}

/* Query 3 (Forma 2) -> Correta */
/* Verified */
void query3_f2(SGV sgv, char* product, int month, int agreggate){
    fflush(stdout);
    PrintableResults pr = getProductSalesAndProfit(sgv, product, month, agreggate);
    GPtrArray** array = toMultipleArrayOfStrings(pr);
    printToFile("query3_All");
    g_ptr_array_foreach(array[0], (GFunc) printf, NULL);
    g_ptr_array_set_free_func(array[0], free);
    g_ptr_array_free (array[0], TRUE);
    free(array);
    destroy_pr(pr);
}
/* Query 4 (Forma 1) -> Correta */
/* Verified */
void query4_f1(SGV sgv, int oneByOne){
    fflush(stdout);
    PrintableResults pr = getProductsNeverBought(sgv, oneByOne);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query4_All");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 4 (Forma 2) -> Correta */
/* Verified */
void query4_f2(SGV sgv, int oneByOne){
    fflush(stdout);
    PrintableResults pr = getProductsNeverBought(sgv, oneByOne);
    GPtrArray** array = toMultipleArrayOfStrings(pr);
    printToFile("query4_B1");
    g_ptr_array_foreach(array[0], (GFunc) printf, NULL);
    fflush(stdout);
    printToFile("query4_B2");
    g_ptr_array_foreach(array[1], (GFunc) printf, NULL);
    fflush(stdout);
    printToFile("query4_B3");
    g_ptr_array_foreach(array[2], (GFunc) printf, NULL);
    free(array);
    destroy_pr(pr);
}

/* Query 5 -> Correta */
/* Verified */
void query5(SGV sgv){
    fflush(stdout);
    PrintableResults pr = getClientsOfAllBranches(sgv);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query5");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}
/* Query 6 -> Correta */
/* VERIFIED -- CORRECT */
void query6(SGV sgv){
    fflush(stdout);
    PrintableResults pr = getClientsAndProductsNeverBoughtCount(sgv);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query6");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 7 -> Correta */
/* VERIFIED -> CORRECT */
void query7(SGV sgv, char* customer){
    fflush(stdout);
    PrintableResults pr = getProductsBoughtByClient(sgv, customer);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query7");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 8 -> Correta */
/* VERIFIED -> CORRECT */
void query8(SGV sgv, int minMonth, int maxMonth){
    fflush(stdout);
    PrintableResults pr = getSalesAndProfit(sgv, minMonth, maxMonth);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query8");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 9 -> Correta */
/* VERIFIED -> CORRECT */
void query9(SGV sgv, char* product, int branch){
    fflush(stdout);
    PrintableResults pr = getProductBuyers(sgv, product, branch);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query9");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 10 -> Correta */
/* VERIFIED -> CORRECT */
void query10(SGV sgv, char* customer, int month){
    fflush(stdout);
    PrintableResults pr = getClientsFavouriteProducts(sgv, customer, month);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query10");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 11 -> Correta */
/* VERIFIED -> CORRECT */
void query11(SGV sgv, int limit){
    fflush(stdout);
    PrintableResults pr = getTopSelledProducts(sgv, limit);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query11");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 12 -> Correta */
/* VERIFIED -> CORRECT */
void query12(SGV sgv, char* clientID, int limit){
    fflush(stdout);
    PrintableResults pr = getClientTopProfitProducts(sgv, clientID, limit);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query12");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

/* Query 13 -> Correta */
/* VERIFIED -> CORRECT */
void query13(SGV sgv){
    fflush(stdout);
    PrintableResults pr = getCurrentFilesInfo(sgv);
    GPtrArray* array = toArrayOfStrings(pr);
    printToFile("query13");
    g_ptr_array_foreach(array, (GFunc) printf, NULL);
    destroy_pr(pr);
}

void do_all_queries(SGV sgv){
    query2(sgv, 'A');
    query3_f1(sgv, "AF1184", 2, 0);
    query3_f2(sgv, "AF1184", 2, 1);
    query4_f1(sgv, 0);
    query4_f2(sgv, 1);
    query5(sgv);
    query6(sgv);
    query7(sgv, "R2292");
    query8(sgv, 1, 6);
    query9(sgv, "AF1184", 1);
    query10(sgv, "R2292", 11);
    query11(sgv, 25);
    query12(sgv, "R2292", 25);
    query13(sgv);
}

int main(int argc, char** argv){
    SGV sgv;

    if(argc >= 2 && !strcmp(argv[1], "--timing-mode")) sgv = initSGVWithTimingOn();
    else sgv = initSGV();

    loadSGVFromFolder(sgv, PATH_TO_FILES);

    if((argc == 2 && !strcmp(argv[1], "--do-queries")) || ((argc == 3) && !strcmp(argv[2], "--do-queries")))
        do_all_queries(sgv);

    destroySGV(sgv);

	return 0;
}
