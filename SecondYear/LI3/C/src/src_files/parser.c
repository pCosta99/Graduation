#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include "sale.h"
#include "config.h"
#include "parser.h"

#define NCHARS_PRODS          2
#define NCHARS_CLIENTES       1
#define BASE_SIZE		      20000
#define EXTRA_SPACE_COEFF     2
#define DEFAULT_SIZE          26
#define UPPER_LIMIT_PRODUCTS  9999
#define LOWER_LIMIT_PRODUCTS  1000
#define UPPER_LIMIT_CLIENTES  5000
#define LOWER_LIMIT_CLIENTES  1000
#define PRODUCT_SIZE          6
#define CUSTOMER_SIZE         5
#define NUM_BRANCHS           3
#define NUM_MONTHS            12

#define VALID_PRODUCT(product) valida(product, 6, 2, "\n\r", 1000, 9000);
#define VALID_CUSTOMER(customer) valida(customer, 5, 1, "\n\r", 1000, 5000);

char regex[2] = {'\r','\n'};

/* Valida produtos e clientes */
int valida(char* string, int size, int charLimit, char* regex, int lower_limit, int upper_limit){
    int i;
	for(i = 0; string[i]; i++){
		/* Garante que até charLimit só existem caracteres maiusculos e o resto são digitos (até size) ou separadores permitidos */
		if((i < size && ((i < charLimit && !isupper(string[i])) || (i >= charLimit && !isdigit(string[i])))) || (i >= size && contains(string[i], regex)))
			return 1;
	}
    int code = atoi(string+charLimit);
	return VALID_NUMBER(code, lower_limit, upper_limit);
}

int valid_product(char* product){
    return valida(product, PRODUCT_SIZE, NCHARS_PRODS, regex, LOWER_LIMIT_PRODUCTS, UPPER_LIMIT_PRODUCTS);
}

int valid_customer(char* customer){
    return valida(customer, CUSTOMER_SIZE, NCHARS_CLIENTES, regex, LOWER_LIMIT_CLIENTES, UPPER_LIMIT_CLIENTES);
}

/* Valida vendas */
Venda validaVenda(char* venda, SGV sgv){
    int nWords = 0;

    sgv_start_timing(sgv, "validaVendaWords");
    char** splited = words(venda, ' ', &nWords);
    sgv_agreggate_timing(sgv, "validaVendaWords");

    if(nWords != 7) return NULL;

    char* prod = splited[0]; /* gets deleted atm, dup later if needed */
    float price = atof(splited[1]); /* might need to be rounded */
    int amount = atoi(splited[2]);
    char type = splited[3][0];
    char* customer = splited[4]; /* gets deleted atm, dup later if needed */
    int month = atoi(splited[5]);
    int filial = atoi(splited[6]);

    sgv_start_timing(sgv, "validaVendaCatalogExists");

    /* Checks all conditions and agregates them in an int */
    int isValid = VALID_NUMBER(price, 0.0, 999.99)
                 +VALID_NUMBER(amount, 1, 200)
                 +VALID_NUMBER(month, 1, 12)
                 +VALID_NUMBER(filial, 1, 3)
                 +(type != 'P' && type != 'N')
                 +sgv_checkIfCustomerExists(sgv, customer)
                 +sgv_checkIfProductExists(sgv, prod);

    sgv_agreggate_timing(sgv, "validaVendaCatalogExists");

    Venda v = NULL;

    if(!isValid) v = createSale(prod, price, amount, type, customer, month, filial);

    /* Free the memory alloc'd */
    int i;
    for(i = 0; splited[i]; i++){
        free(splited[i]);
    }
    free(splited);

    return v;
}

int loadCatalog(SGV sgv, char* path, int type){
	int valid = 0, depth, count = 0;
	FILE * f;

	/* Check if file exists */
	if(!(f = fopen(path, "r"))){
		printf("O ficheiro inserido não existe.\n");
		return 1;
	}

    /* Alloc space to read lines */
    int size_line = EXTRA_SPACE_COEFF * (!type ? PRODUCT_SIZE : CUSTOMER_SIZE);
	char* line = malloc(size_line);

	/* Prepare the depth of the dictionary according to the file we are reading */
	!type ? (depth = NCHARS_PRODS) : (depth = NCHARS_CLIENTES);

    /* Start timing the process */
	!type ? sgv_start_timing(sgv, "loadCatalogProducts") : sgv_start_timing(sgv, "loadCatalogClientes");

	/* Proceed to read the file */
	while(fgets(line, size_line, f)){
		/* If the line doesn't end in newline or \0 keep reading until it does */
		int last = strcspn(line, "\n\0");
		while(line[last] != '\n' && line[last] != '\0') {
			memset(line, 0, size_line * EXTRA_SPACE_COEFF);
			fgets(line, size_line * EXTRA_SPACE_COEFF, f);
			if(!line[0]) break;
		}

		/* If the line is valid store it in the catalog */
		if((type == 0 && !valid_product(line))
		|| (type == 1 && !valid_customer(line))){
		    /* +1 to valid */
		    valid++;

            char* string = my_strdup(line);
            int last = strcspn(string, "\r\n\0");
            string[last] = 0;
	        !type ? sgv_addProductToCatalog(sgv, string) : sgv_addCustomerToCatalog(sgv,string);
            !type ? sgv_addEmptyProductToFac(sgv, string) : sgv_addEmptyCustomerToBranches(sgv, string);
        }
        /* +1 to total */
		count++;
	}

	/* Finish timing */
	!type ? sgv_finish_timing(sgv, "loadCatalogProducts") : sgv_finish_timing(sgv, "loadCatalogClientes");

	/* Mark the total and valid numbers */
	!type ? sgv_setTotalProducts(sgv, count) : sgv_setTotalCustomers(sgv, count);
	!type ? sgv_setValidProducts(sgv, valid) : sgv_setValidCustomers(sgv, valid);

	free(line);
	fclose(f);

	return 0;
}

void loadVendas(SGV sgv, char* path, int size_line){
	int valid = 0, total = 0;
	FILE * f;

	/* Check if file exists */
	if(!(f = fopen(path, "r"))){
		printf("O ficheiro inserido não existe.\n");
	}

    /* Alloc space to read lines */
	char* line = malloc(size_line * EXTRA_SPACE_COEFF);

    /* Start timing the process */
    sgv_start_timing(sgv, "loadVendas");

	/* Proceed to read the file */
	while(fgets(line, (size_line * EXTRA_SPACE_COEFF), f)){
		/* If the line doesn't end in newline or \0 keep reading until it does */
		int last = strcspn(line, "\n\0");
		while(line[last] != '\n' && line[last] != '\0') {
			memset(line, 0, size_line * EXTRA_SPACE_COEFF);
			fgets(line, size_line * EXTRA_SPACE_COEFF, f);
			if(!line[0]) break;
		}

        /* Declare a sale and fill it */
        Venda v = NULL;

		/* If the line is valid store it in the catalog */
		if((v = validaVenda(line, sgv)) != NULL){
            sgv_start_timing(sgv, "billProduct");
            sgv_billProduct(sgv, v);
            sgv_agreggate_timing(sgv, "billProduct");

            sgv_start_timing(sgv, "customerBought");
            sgv_customerBought(sgv, v);
            sgv_agreggate_timing(sgv, "customerBought");

            valid++;
        }

        total++;
        if(v) destroy_venda(v);
	}

    /* Finishes timing the process */
    sgv_finish_timing(sgv, "billProduct");
    sgv_finish_timing(sgv, "customerBought");
    sgv_finish_timing(sgv, "loadVendas");
    sgv_finish_timing(sgv, "validaVendaWords");
    sgv_finish_timing(sgv, "validaVendaCatalogExists");

    setValidSales(sgv, valid);
    setTotalSales(sgv, total);

	free(line);
	fclose(f);
}
