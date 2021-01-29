#ifndef __MENU__
#define __MENU__

#include <glib.h>
#include "navegador.h"

/* clear_screen = 1 means clear screen */
#define CLEAR_SCREEN printf("\e[1;1H\e[2J");


/* Prints the current page of navigator to the screen */
void print_navigator_current_page(Navigator nav, int adjust);

/* Prints a GPtrArray of strings */
void view_regular_strings(GPtrArray* array, int clear_screen);

/* View for query 3 */
void view_query3(char* product, int month, int branch, int v_N, int v_P, float p_N, float p_P, int clear_screen);

/* View for query 6 */
void view_query6(int products, int customers, int clear_screen);

/* View for query 7 */
void view_query7(int** monthlyAmounts, int clear_screen);

/* View for query 8 */
void view_query8(int fst_month, int snd_month, long total_vendas, float total_faturado, int clear_screen);

/* Transforms query 9 info into a printable view for navigator */
GPtrArray* convert_query9(void* ptr);

/* Transforms query 10 info into a printable view for navigator */
GPtrArray* convert_query10(void* ptr);

/* Transforms query 11 info into a printable view for navigator */
GPtrArray* convert_query11(void* ptr);

/* Transforms query 12 info into a printable view for navigator */
GPtrArray* convert_query12(void* ptr);

/* View for query 13 */
void view_query13(char** paths, int* valid, int* total, int clear_screen);

/* Mostra o menu principal ao utilizador */
void mostra_menu();

#endif
