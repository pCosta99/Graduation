
#ifndef __CATALOG__
#define __CATALOG__

#include <gmodule.h>

 /* Opaque data type defined in this module */
typedef struct catalog *Catalog;

/* Initializes the catalog */
Catalog init_catalog(int size, int depth, int range, int lower_limit, void (*f)(void*));

/* Destroys the catalog and free's the memory it retained */
void destroy_catalog(Catalog c);

/* Returns NULL if the key didn't existed, returns the value associated with the key oherwise */
void* catalog_get(Catalog c, char* string_code);

/* Checks if a code exists in some letters tree */
int catalog_exists(Catalog c, char* string_code);

/* Inserts a code in some letters tree */
int catalog_insert(Catalog c, char* string_code, void* value);

/* Removes a code from some letters tree */
int catalog_remove(Catalog c, char* string_code);

/* Performs a traverse through all catalog */
void catalog_foreach(Catalog c, gboolean (*traverse_func)(void*, void*, void*), void* data);

/* Performs a traverse through one section of the catalog */
void catalog_foreach_in_letter_range(Catalog c, char letter, gboolean (*traverse_func)(void*, void*, void*), void* data);

#endif
