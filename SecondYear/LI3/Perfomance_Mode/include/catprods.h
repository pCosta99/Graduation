#ifndef __PRODUCT_CATALOG__
#define __PRODUCT_CATALOG__

typedef struct catprods *CatProds;

/*Inits the product catalog*/
CatProds initCatProds();

/*Destroys the product catalog*/
void destroyCatProds(CatProds c);

/*Inserts a product in the catalog*/
void insertProduct(CatProds c, char* product);

/*Checks if a product exists in the catalog. 0 means yes*/
int productExists(CatProds c, char* product);

/*Retrieves all products that start with a certain letter*/
GPtrArray* retrieve_letter_products(CatProds c, char letter);

/*Returns all the products the catalog contains*/
GPtrArray* allProducts(CatProds c);

int getTotalProducts(CatProds c);

void setTotalProducts(CatProds c, int totalProducts);

int getValidProducts(CatProds c);

void setValidProducts(CatProds c, int validProducts);

#endif
