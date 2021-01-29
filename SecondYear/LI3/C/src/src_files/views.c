#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "pair.h"
#include "pqueue.h"
#include "views.h"

void print_navigator_current_page(Navigator nav, int adjust){
    CLEAR_SCREEN;
    print_content(nav, adjust);
}

void printf_with_newline(gpointer data, gpointer user_data){
    printf("%s\n", (char*) data);
}

void view_regular_strings(GPtrArray* array, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    g_ptr_array_foreach(array, printf_with_newline, NULL);
}

void view_query3(char* product, int month, int branch, int v_N, int v_P, float p_N, float p_P, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    if(branch){
        printf("Filial: %d\n\n", branch);
    }
    printf("Produto: %s\n", product);
    printf("Mês: %d\n\n", month);
    printf("Número de vendas normais: %d\n", v_N);
    printf("Número de vendas em promoção: %d\n",v_P);
    printf("Total faturado fora de promoção: %.2f\n", p_N);
    printf("Total faturado em promoção: %.2f\n", p_P);
}

void view_query6(int products, int customers, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    printf("Número de produtos que ninguém comprou: %d\n", products);
    printf("Número de clientes que nunca compraram: %d\n", customers);
}

void view_query7(int** monthlyAmounts, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    int i, j;

    for(i = 0; monthlyAmounts[i]; i++){
        for(j = 0; monthlyAmounts[i][j] != -1; j++){
            printf("%d ", monthlyAmounts[i][j]);
        }
        printf("\n");
    }
}

void view_query8(int fst_month, int snd_month, long total_vendas, float total_faturado, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    printf("Entre os meses %d e %d obtiveram-se os seguintes resultados:\n\n", fst_month, snd_month);
    printf("Total faturado: %.2f\n", total_faturado);
    printf("Total vendido: %ld\n", total_vendas);
}

GPtrArray* convert_query9(void* ptr){
    int i;
    Pair p = (Pair) ptr;
    GPtrArray* array = g_ptr_array_new_with_free_func(free);
    GPtrArray* regular = (GPtrArray*) fst(p);
    GPtrArray* promotion = (GPtrArray*) snd(p);

    char* data = malloc(sizeof(char) * 100);
    sprintf(data, "Em modo normal:");
    g_ptr_array_add(array, my_strdup(data));
    for(i = 0; i < regular->len; i++) {
        sprintf(data, "%s", (char*) g_ptr_array_index(regular, i));
        g_ptr_array_add(array, my_strdup(data));
    }
    sprintf(data, "\nEm promoção:");
    g_ptr_array_add(array, my_strdup(data));
    for(i = 0; i < promotion->len; i++) {
        sprintf(data, "%s", (char*) g_ptr_array_index(promotion, i));
        g_ptr_array_add(array, my_strdup(data));
    }
    free(data);

    return array;
}

GPtrArray* convert_query10(void* ptr){
    PQueue* heap = (PQueue*) ptr;
    GPtrArray* array = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 100);

    char* n = (char*) pqueue_dequeue(heap);
    while(n){
        sprintf(data, "%s", n);
        g_ptr_array_add(array, my_strdup(data));
        free(n);
        n = (char*) pqueue_dequeue(heap);
    }
    free(data);
    free(n);

    return array;
}

GPtrArray* convert_query11(void* ptr){
    int i;
    GPtrArray* pairs = (GPtrArray*) ptr;
    GPtrArray* array = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 500);

    for(i = 0; i < pairs->len; i++){
        Pair p = (Pair) g_ptr_array_index(pairs, i);
        char* product = (char*) fst(p);
        int** product_info = (int**) snd(p);

        sprintf(data, "Product: %s, n_buyers: { tot: %d, f1: %d, f2: %d, f3: %d }, amount: { tot: %d, f1: %d, f2: %d, f3: %d }",product, product_info[0][0], product_info[1][0], product_info[2][0], product_info[3][0], product_info[0][1], product_info[1][1], product_info[2][1], product_info[3][1]);
        g_ptr_array_add(array, my_strdup(data));
    }

    free(data);

    return array;
}

GPtrArray* convert_query12(void* ptr){
    GPtrArray* pairs = (GPtrArray*) ptr;
    GPtrArray* array = g_ptr_array_new_with_free_func(free);
    char* data = malloc(sizeof(char) * 100);
    int i;

    for(i = 0; i < pairs->len; i++){
        Pair p = (Pair) g_ptr_array_index(pairs, i);
        char* product = fst(p);
        float* amount_bought = snd(p);

        sprintf(data, "%s : %.2f", product, *amount_bought);
        g_ptr_array_add(array, my_strdup(data));
    }

    free(data);

    return array;
}

/* [0] == products, [1] == customers, [2] == sales */
void view_query13(char** paths, int* valid, int* total, int clear_screen){
    if(clear_screen) CLEAR_SCREEN;
    int products = 0, customers = 1, sales = 2;

    /* Sales info */
    printf("Vendas:\n\n");
    printf("Ficheiro: %s\n", paths[sales]);
    printf("Vendas válidas: %d\n", valid[sales]);
    printf("Vendas inválidas: %d\n", (total[sales] - valid[sales]));
    printf("Total de vendas: %d\n", total[sales]);

    /* Products info */
    printf("\nProdutos:\n\n");
    printf("Ficheiro: %s\n", paths[products]);
    printf("Produtos válidos: %d\n", valid[products]);
    printf("Produtos inválidos: %d\n", (total[products] - valid[products]));
    printf("Total de produtos: %d\n", total[products]);

    /* Customers info */
    printf("\nClientes:\n\n");
    printf("Ficheiro: %s\n", paths[customers]);
    printf("Clientes válidos: %d\n", valid[customers]);
    printf("Clientes inválidos: %d\n", (total[customers] - valid[customers]));
    printf("Total de clientes: %d\n", total[customers]);
}

void mostra_menu(){
    CLEAR_SCREEN;

	printf("Bem-Vindo ao SGV.\n");
	printf("Por favor selecione a query a executar:\n");
	printf("1 - Leitura de ficheiros (Produtos, Clientes e Vendas)\n");
	printf("2 - Lista de produtos começados por dada letra\n");
	printf("3 - Numero total de vendas e total faturado de um dado produto num dado mes\n");
	printf("4 - Lista dos produtos não vendidos\n");
	printf("5 - Lista de clientes que compraram em todas as filiais\n");
	printf("6 - Numero de clientes que nao realizaram compras e produtos nao comprados\n");
	printf("7 - Tabela com numero total de produtos comprados ao longo dos meses e nas distintas filiais por um cliente\n");
	printf("8 - Total de vendas e total faturado num intervalo de meses\n");
	printf("9 - Codigo e numero de clientes que compraram um produto numa filial\n");
	printf("10 - Lista de produtos que um cliente mais comprou num mes por ordem decrescente\n");
	printf("11 - Lista dos produtos mais vendidos no ano\n");
	printf("12 - Produtos em que o cliente mais gastou no ano\n");
	printf("13 - Apresentaçao da leitura de ficheiros (Produtos, Clientes e Vendas)\n");
	printf("14 - Informação acerca de um dado produto\n");
	printf("0 - Sair\n");

    /* Flush stdout just in case */
    fflush(stdout);
}
