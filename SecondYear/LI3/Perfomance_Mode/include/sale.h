#ifndef __SALE__
#define __SALE__

typedef struct sale* Venda;

Venda createSale(char* product, float price, int amount, char type, char* customer, int month, int branch);

char* getProduct(Venda v);

float getPrice(Venda v);

int getAmount(Venda v);

char getType(Venda v);

char* getCustomer(Venda v);

int getMonth(Venda v);

int getBranch(Venda v);

void setProduct(Venda v, char* product);

void setPrice(Venda v, float price);

void setAmount(Venda v, int amount);

void setType(Venda v, char type);

void setCustomer(Venda v, char* customer);

void setMonth(Venda v, int month);

void setBranch(Venda v, int branch);

void destroy_venda(Venda v);

#endif
