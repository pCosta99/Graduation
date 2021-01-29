#include "config.h"
#include "catalog.h"
#include "catprods.h"

struct catprods{
    Catalog products;
    int validProducts, totalProducts;
};

CatProds initCatProds(){
    CatProds c = malloc(sizeof(struct catprods));
    c->products = init_catalog(26, 2, 9000, 1000, free);
    return c;
}

void destroyCatProds(CatProds c){
    destroy_catalog(c->products);
    free(c);
}

void insertProduct(CatProds c, char* product){
    catalog_insert(c->products, product, product);
}

int productExists(CatProds c, char* product){
    return catalog_exists(c->products, product);
}

GPtrArray* retrieve_letter_products(CatProds c, char letter){
    GPtrArray* array = g_ptr_array_new_with_free_func(free);

    catalog_foreach_in_letter_range(c->products, letter, (GTraverseFunc) insert_in_array, array);

    return array;
}

GPtrArray* allProducts(CatProds c){
    GPtrArray* array = g_ptr_array_new_with_free_func(free);

    catalog_foreach(c->products, (GTraverseFunc) insert_in_array, array);

    return array;
}

int getTotalProducts(CatProds c){
    return c->totalProducts;
}

void setTotalProducts(CatProds c, int totalProducts){
    c->totalProducts = totalProducts;
}

int getValidProducts(CatProds c){
    return c->validProducts;
}

void setValidProducts(CatProds c, int validProducts){
    c->validProducts = validProducts;
}
