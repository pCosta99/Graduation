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
#define VALID_NUMBER(I, B, C) !(I >= B && I <= C)
#define DEFAULT_SIZE          26
#define UPPER_LIMIT_PRODUCTS  9999
#define LOWER_LIMIT_PRODUCTS  1000
#define UPPER_LIMIT_CLIENTES  5000
#define LOWER_LIMIT_CLIENTES  1000
#define NUM_BRANCHS           3
#define NUM_MONTHS            12

/* Useful for measuring times */
Stats s;

/* Valida vendas */
Venda validaVenda(char* venda, SGV sgv){
    int nWords = 0;

    start_timing(s, "validaVendaWords");
    char** splited = words(venda, ' ', &nWords);
    agreggate_timing(s, "validaVendaWords");

    if(nWords != 7) return NULL;

    char* prod = splited[0]; /* gets deleted atm, dup later if needed */
    float price = atof(splited[1]); /* might need to be rounded */
    int amount = atoi(splited[2]);
    char type = splited[3][0];
    char* customer = splited[4]; /* gets deleted atm, dup later if needed */
    int month = atoi(splited[5]);
    int filial = atoi(splited[6]);

    start_timing(s, "validaVendaCatalogExists");

    /* Checks all conditions and agregates them in an int */
    int isValid = VALID_NUMBER(price, 0.0, 999.99)
                 +VALID_NUMBER(amount, 1, 200)
                 +VALID_NUMBER(month, 1, 12)
                 +VALID_NUMBER(filial, 1, 3)
                 +(type != 'P' && type != 'N')
                 +sgv_checkIfCustomerExists(sgv, customer)
                 +sgv_checkIfProductExists(sgv, prod);

    agreggate_timing(s, "validaVendaCatalogExists");

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

int loadCatalog(SGV sgv, char* path, int size_line, int type){
	int valid = 0, depth, count = 0;
	FILE * f;
	char regex[2] = {'\r','\n'};

	/* Check if file exists */
	if(!(f = fopen(path, "r"))){
		printf("O ficheiro inserido não existe.\n");
		return 1;
	}

    /* Alloc space to read lines */
	char* line = malloc(size_line * EXTRA_SPACE_COEFF);

	/* Prepare the depth of the dictionary according to the file we are reading */
	!type ? (depth = NCHARS_PRODS) : (depth = NCHARS_CLIENTES);

    /* Start timing the process */
    s = getStats(sgv);
	!type ? start_timing(s, "loadCatalogProducts") : start_timing(s, "loadCatalogClientes");

	/* Proceed to read the file */
	while(fgets(line, (size_line * EXTRA_SPACE_COEFF), f)){
		/* If the line doesn't end in newline or \0 keep reading until it does */
		int last = strcspn(line, "\n\0");
		while(line[last] != '\n' && line[last] != '\0') {
			memset(line, 0, size_line * EXTRA_SPACE_COEFF);
			fgets(line, size_line * EXTRA_SPACE_COEFF, f);
			if(!line[0]) break;
		}

		/* If the line is valid store it in the catalog */
		if((type == 0 && !valida(line, size_line, NCHARS_PRODS, regex, LOWER_LIMIT_PRODUCTS,UPPER_LIMIT_PRODUCTS))
		|| (type == 1 && !valida(line, size_line, NCHARS_CLIENTES, regex, LOWER_LIMIT_CLIENTES, UPPER_LIMIT_CLIENTES))){
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
	!type ? finish_timing(s, "loadCatalogProducts") : finish_timing(s, "loadCatalogClientes");

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
    s = getStats(sgv);
    start_timing(s, "loadVendas");

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
            start_timing(s, "billProduct");
            sgv_billProduct(sgv, v);
            agreggate_timing(s, "billProduct");

            start_timing(s, "customerBought");
            sgv_customerBought(sgv, v);
            agreggate_timing(s, "customerBought");

            valid++;
        }

        total++;
        if(v) destroy_venda(v);
	}

    /* Finishes timing the process */
    finish_timing(s, "billProduct");
    finish_timing(s, "customerBought");
    finish_timing(s, "loadVendas");
    finish_timing(s, "validaVendaWords");
    finish_timing(s, "validaVendaCatalogExists");

    setValidSales(sgv, valid);
    setTotalSales(sgv, total);

	free(line);
	fclose(f);
}
