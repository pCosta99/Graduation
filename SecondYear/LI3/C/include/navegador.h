#ifndef __NAVIGATOR__
#define __NAVIGATOR__

#include <glib.h>

typedef struct navigator *Navigator;

/* Inicializa o navegador */
Navigator init_navigator(GPtrArray* buffer);

/* Free do navegador */
void destroy_navigator(Navigator nav);

/* Allows the user to change the number of columns to print */
void set_cols_number(Navigator nav, int cols_number);

/* Devolve a página anterior */
void getPreviousPage(Navigator nav);

/* Devolve a próxima página */
void getNextPage(Navigator nav);

/* Printa o conteudo. 1 means yes */
void print_content(Navigator nav, int adjust);

#endif
