#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "views.h"
#include "parser.h"
#include "navegador.h"
#include "config.h"
#include "pair.h"
#include "pqueue.h"
#include "controller.h"

/* Not counting the initialization one */
/* Counting prints of faturacao and filial */
#define NUM_QUERIES 13
#define SLEEP_TIME  2
#define PATH_TO_RESULTS    "Stats/Results"

#define ERROR_MESSAGE(message, ret) { printf("%s\n", message); fflush(stdout); sleep(SLEEP_TIME); return ret; }
#define ERROR_MESSAGE_VOID_RETURN(message) { printf("%s\n", message); fflush(stdout); sleep(SLEEP_TIME); return; }
#define CHECK_IF_SGV_IS_LOADED(sgv) { if(!isLoaded(sgv)) { printf("O SGV não está carregado.\n"); fflush(stdout); sleep(SLEEP_TIME); return; } }

int stdout_dup, save_queries, timing_mode_on;

/* Declare the type of a parse query */
typedef void Parse_Query(SGV);

void ignore_user_input(){
    char trash[100];
    scanf("%s", trash); /* read whatever they decide to write */
    getchar(); /* Catch \n */
}

void printToFile(char* queryName){
    char* path = malloc(sizeof(char) * 100);
    char* temp = get_results_path();
    sprintf(path, "%s/%s", temp, queryName);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    stdout_dup = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    free(path);
    free(temp);
}

void printToStdOut(){
    /* make stdout right and close the copy */
    fflush(stdout);
    dup2(stdout_dup, STDOUT_FILENO);
    close(stdout_dup);
}

SGV parse_query1(SGV sgv){
    char customer_path[100], products_path[100], sales_path[100];
    FILE* f;

    printf("Caminho para o ficheiro de clientes:\n"); fflush(stdout);
    scanf("%s", customer_path); getchar();
    f = fopen(customer_path, "r");
    if(f == NULL) { ERROR_MESSAGE("O ficheiro referido não existe.", sgv); }
    else fclose(f);

    printf("Caminho para o ficheiro de produtos:\n"); fflush(stdout);
    scanf("%s", products_path); getchar();
    f = fopen(products_path, "r");
    if(f == NULL) { ERROR_MESSAGE("O ficheiro referido não existe.", sgv); }
    else fclose(f);

    printf("Caminho para o ficheiro de vendas:\n"); fflush(stdout);
    scanf("%s", sales_path); getchar();
    f = fopen(sales_path, "r");
    if(f == NULL) { ERROR_MESSAGE("O ficheiro referido não existe.", sgv); }
    else fclose(f);

    if(isLoaded(sgv)) {
        timing_mode_on = isTimingModeOn(sgv);
        destroySGV(sgv);

        sgv = initSGV();
        if(timing_mode_on) setTimingModeOn(sgv);

        if(save_queries) turnOnQuerySavingMode(sgv);
    }
    sgv = loadSGVFromFiles(sgv, customer_path, products_path, sales_path);

    return sgv;
}

int parse_upper_letter(char *letter){
    letter[0] = letter[1] = 0;
    printf("Introduza a letra que deseja procurar:\n"); fflush(stdout);
    scanf("%s", letter); getchar();

    /* Ensure data is correct or warn the user it isn't */
    if(!isupper(*letter)) ERROR_MESSAGE("A letra tem de ser maiuscula.", 0);

    return 1;
}

int parse_product(SGV sgv, char* product){
    printf("Introduza o produto sobre o qual deseja obter informação.\n"); fflush(stdout);
    scanf("%s", product); getchar();

    /* Ensure data is correct or warn the user it isn't */
    if(valid_product(product)) ERROR_MESSAGE("Produto inválido.", 0);
    if(sgv_checkIfProductExists(sgv, product)) ERROR_MESSAGE("Produto inexistente.", 0);

    return 1;
}

int parse_customer(SGV sgv, char* customer){
    printf("Introduza o código do cliente sobre o qual quer obter informação.\n"); fflush(stdout);
    scanf("%s", customer); getchar();

    /* Ensure data is correct */
    if(valid_customer(customer)) ERROR_MESSAGE("Cliente inválido.", 0);
    if(sgv_checkIfCustomerExists(sgv, customer)) ERROR_MESSAGE("Cliente inexistente.", 0);

    return 1;
}

int parse_month(int *month){
    printf("Introduza o mês em que deseja efetuar a pesquisa.\n"); fflush(stdout);
    scanf("%d", month); getchar();

    /* Ensure data is correct or warn the user it isn't */
    if(VALID_NUMBER(*month, 1, 12)) ERROR_MESSAGE("Mês inválido.", 0);

    return 1;
}

void parse_flag(int *flag){
    printf("Deseja agregar a informação obtida?\n0 para não agregar, qualquer outro valor em caso contrário.\n"); fflush(stdout);
    scanf("%d", flag); getchar();
}

int parse_branch(int *branch){
    printf("Introduza a filial na qual deseja obter resultados.\n"); fflush(stdout);
    scanf("%d", branch); getchar();

    /* Ensure data is correct */
    if(VALID_NUMBER(*branch, 1, 3)) ERROR_MESSAGE("Valor da filial inválido.", 0);

    return 1;
}

int parse_int(int *value, int lower_limit, int upper_limit){
    printf("Introduza o número de resultados que deseja obter.\n"); fflush(stdout);
    scanf("%d", value); getchar();

    /* Ensure data is correct */
    if(VALID_NUMBER(*value, lower_limit, upper_limit)) ERROR_MESSAGE("Valor inválido.", 0);

    return 1;
}

void loop_navigator(GPtrArray* info, int cols_number, char* file_name){
    /* If we are supposed to save the queries into files do it now */
    if(save_queries) { printToFile(file_name); view_regular_strings(info, 0); printToStdOut(); }

    /* Create the navigator */
    Navigator nav = init_navigator(info);

    /* Loop through the navigator and keep sending data to the display view */
    char c[100];
    *c = 0;
    while(*c != 'q'){
        if(*c == 'n') getNextPage(nav);
        else if(*c == 'p') getPreviousPage(nav);

        /* Update view with new info */
        if(cols_number) {
            set_cols_number(nav, cols_number);
            print_navigator_current_page(nav, 0);
        } else print_navigator_current_page(nav, 1);

        /* Wait for next action */
        scanf("%s", c); getchar();
    }

    /* Free the navigator */
    destroy_navigator(nav);
}

/* Generic way of handling data */
void handle_data(void* data, int column_size, GPtrArray* (*convert)(void*), void (*free_func)(void*), char* file_name){
    GPtrArray* array;
    /* Convert the data into printable info and send it to the navigator */
    if(convert) array = convert(data);
    else array = (GPtrArray*) data;

    loop_navigator(array, column_size, file_name);

    /* Free all memory */
    if(free_func) free_func(data);
    g_ptr_array_free(array, TRUE);
}

/* --------------------------- QUERY 2 ------------------------------- */
void parse_query2(SGV sgv){
    char letter[2];

    /* Parse */
    if(!parse_upper_letter(letter)) return;

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the products */
    GPtrArray* array = getProductsStartedByLetter(sgv, *letter);
    handle_data(array, 0, NULL, NULL, "query2");
}

/* --------------------------- QUERY 3 ------------------------------- */
void parse_query3(SGV sgv){
    char product[100], filename[100];
    int month[1], flag[1];

    /* Parse */
    if(!parse_product(sgv, product) || !parse_month(month)) return;

    parse_flag(flag);

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    Query3 *q = getProductSalesAndProfit(sgv, product, *month, *flag);

    *flag = *flag ? 1 : 0;
    int i;
    for(i = 0; q[i]; i++){
        if (*flag) sprintf(filename, "query3_total");
        else sprintf(filename, "query3_B%d", i+1);

        /* If we are supposed to save the queries into files do it now */
        if(save_queries) { printToFile(filename); view_query3(product, *month, i+1, q[i]->vendas_N, q[i]->vendas_P, q[i]->profit_N, q[i]->profit_P, 0); printToStdOut(); }

        /* Update the view */
        if(*flag) view_query3(product, *month, i, q[i]->vendas_N, q[i]->vendas_P, q[i]->profit_N, q[i]->profit_P, 1);
        else view_query3(product, *month, i+1, q[i]->vendas_N, q[i]->vendas_P, q[i]->profit_N, q[i]->profit_P, 1);

        /* Wait for user input before continuing */
        char c = getchar();
        while (c != '\n' && (getchar()) != '\n');

        /* Free values */
        free(q[i]->product); free(q[i]);
    }
    free(q);
}

/* --------------------------- QUERY 4 ------------------------------- */
void parse_query4(SGV sgv){
    int flag[1]; char filename[100];

    /* Parse */
    parse_flag(flag);
    *flag = *flag ? 0 : 1;

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    GPtrArray** arrays = getProductsNeverBought(sgv, *flag);

    int i;
    for(i = 0; arrays[i]; i++){
        if (*flag) sprintf(filename, "query4_total");
        else sprintf(filename, "query4_B%d", i+1);
        /* Send the results into the navigator to be printed */
        loop_navigator(arrays[i], 0, filename);
        g_ptr_array_free(arrays[i], TRUE);
    }
    free(arrays);
}

/* --------------------------- QUERY 5 ------------------------------- */
void parse_query5(SGV sgv){
    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    GPtrArray* array = getClientsOfAllBranches(sgv);
    handle_data(array, 0, NULL, NULL, "query5");
}

/* --------------------------- QUERY 6 ------------------------------- */
void parse_query6(SGV sgv){
    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    int* values = getClientsAndProductsNeverBoughtCount(sgv);

    /* If we are supposed to save the queries into files do it now */
    if(save_queries) { printToFile("query6"); view_query6(values[0], values[1], 0); printToStdOut(); }

    /* Update the view */
    view_query6(values[0], values[1], 1);

    /* Wait for user input before continuing */
    ignore_user_input();

    /* Free values */
    free(values);
}

/* --------------------------- QUERY 7 ------------------------------- */
void parse_query7(SGV sgv){
    char customer[100];

    /* Parse */
    if(!parse_customer(sgv, customer)) return;

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    int** monthlyAmounts = getProductsBoughtByClient(sgv, customer);

    /* If we are supposed to save the queries into files do it now */
    if(save_queries) { printToFile("query7"); view_query7(monthlyAmounts, 0); printToStdOut(); }

    /* Update the view */
    view_query7(monthlyAmounts, 1);

    /* Wait for user input before continuing */
    ignore_user_input();

    /* Free values */
    free(monthlyAmounts);
}

/* --------------------------- QUERY 8 ------------------------------- */
void parse_query8(SGV sgv){
    int min_month[1], max_month[1];

    /* Parse */

    if(!parse_month(min_month) || !parse_month(max_month)) return;

    /* Correct order if needed */
    if(*min_month > *max_month) ERROR_MESSAGE_VOID_RETURN("Introduza primeiro o valor mais baixo.");

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    Pair p = getSalesAndProfit(sgv, *min_month, *max_month);
    long total_vendas = *((long*) fst(p));
    float total_profit = *((float*) snd(p));

    /* If we are supposed to save the queries into files do it now */
    if(save_queries) { printToFile("query8"); view_query8(*min_month, *max_month, total_vendas, total_profit, 0); printToStdOut(); }

    /* Update the view */
    view_query8(*min_month, *max_month, total_vendas, total_profit, 1);

    /* Wait for user input before continuing */
    ignore_user_input();

    /* Free values */
    freePair(p);
}

/* --------------------------- QUERY 9 ------------------------------- */
void parse_query9(SGV sgv){
    char product[100];
    int branch[1];

    /* Parse */
    if(!parse_branch(branch) || !parse_product(sgv, product)) return;

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    Pair p = getProductBuyers(sgv, product, *branch);
    handle_data(p, 1, convert_query9, freePair, "query9");
}

/* --------------------------- QUERY 10 ------------------------------- */
void parse_query10(SGV sgv){
    char customer[100];
    int month[1];

    /* Parse */
    if(!parse_month(month) || !parse_customer(sgv, customer)) return;

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    PQueue* heap = getClientsFavouriteProducts(sgv, customer, *month);
    handle_data(heap, 1, convert_query10, pqueue_delete, "query10");
}

/* --------------------------- QUERY 11 ------------------------------- */
void parse_query11(SGV sgv){
    int value[1];

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Parse */
    if(!parse_int(value, 0, getNumberOfValidProducts(sgv))) return;

    /* Get the results */
    GPtrArray* pairs = getTopSelledProducts(sgv, *value);
    handle_data(pairs, 1, convert_query11, free_gptr_array, "query11");
}

/* --------------------------- QUERY 12 ------------------------------- */
void parse_query12(SGV sgv){
    char customer[100];
    int limit[1];

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Parse */
    if(!parse_int(limit, 0, getNumberOfValidProducts(sgv)) || !parse_customer(sgv, customer)) return;

    /* Get the results */
    GPtrArray* pairs = getClientTopProfitProducts(sgv, customer, *limit);
    handle_data(pairs, 1, convert_query12, free_gptr_array, "query12");
}

/* --------------------------- QUERY 13 ------------------------------- */
void free_query13(void* ptr){
    Query13 q = (Query13) ptr;

    free(q->customersPath); free(q->productsPath); free(q->salesPath); free(q);
}

void parse_query13(SGV sgv){
    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Get the results */
    Query13 q = getCurrentFilesInfo(sgv);

    /* Fix results */
    char* paths[3] = { q->productsPath, q->customersPath, q->salesPath };
    int valid[3] = { q->validProducts, q->validCustomers, q->validSales };
    int total[3] = { q->totalProducts, q->totalCustomers, q->totalSales };

    /* If we are supposed to save the queries into files do it now */
    if(save_queries) { printToFile("query13"); view_query13(paths, valid, total, 0); printToStdOut(); }

    /* Update the view */
    view_query13(paths, valid, total, 1);

    /* Wait for user input before continuing */
    ignore_user_input();

    /* Free values */
    free_query13(q);
}

void parse_product_info(SGV sgv){
    char product[100];

    /* Ensure SGV is loaded */
    CHECK_IF_SGV_IS_LOADED(sgv);

    /* Parse */
    if(!parse_product(sgv, product)) return;

    /* Get the results */
    GPtrArray* productInfo = sgv_productInfoAsString(sgv, product);

    /* Update the view */
    view_regular_strings(productInfo, 1);

    /* Wait for user input before continuing */
    ignore_user_input();

    /* Free values */
    g_ptr_array_free(productInfo, TRUE);
}

/* --------------------------- DEFAULT -------------------------------- */
void parse_query_default(SGV sgv){
    system("clear");

    printf("Invalid option.\nRedirecting to menu.\n");
    fflush(stdout);
    sleep(2);
}

SGV main_loop(SGV sgv){
    int option = NUM_QUERIES+2;
    save_queries = isQuerySavingModeOn(sgv);

    /* Declare an array that contains the functions to parse a certain query */
    Parse_Query *pq[NUM_QUERIES+1] = { parse_query2, parse_query3, parse_query4, parse_query5
                                     , parse_query6, parse_query7, parse_query8, parse_query9, parse_query10
                                     , parse_query11, parse_query12, parse_query13, parse_product_info
                                     , parse_query_default };

    /* While the option is not 0 keep handling it */
    mostra_menu(); scanf("%d", &option); getchar(); /* Catch the implicit \n */

    while(option){
        /* Clear the screen */
        CLEAR_SCREEN;
        /* Choose the appropriate function to deal with the option */
        if(option == 1) sgv = parse_query1(sgv);
        else if(option <= NUM_QUERIES+1){
            pq[option-2](sgv);
        } else pq[NUM_QUERIES](sgv);

        option = NUM_QUERIES+2; /* Reset option */
        mostra_menu(); scanf("%d", &option); getchar(); /* Catch the implicit \n */
    }

    return sgv;
}

