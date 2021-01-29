#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interface.h"
#include "controller.h"
#include "config.h"

SGV parse_options(char** argv){
    SGV sgv;

    /* Init config with a file if it exists, otherwise the defaults will be loaded */
    char* temp = getSuffixOf("--config", argv);
    initConfig(temp);
    free(temp);

    /* Should we time everything? */
    sgv = initSGV();
    if(!existsCode("--timing-mode", argv)) setTimingModeOn(sgv);

    /* Should we do queries before allowing the user to handle the program? */
    if(!existsCode("--save-queries", argv)) turnOnQuerySavingMode(sgv);

    /* Should we pre-load the program with data from the default files? */
    if(!existsCode("--default-load", argv)) {
        char *customers = get_customers_path(), *products = get_products_path(), *sales = get_sales_path();
        sgv = loadSGVFromFiles(sgv, customers, products, sales);
        free(customers); free(products); free(sales);
    }

    return sgv;
}

int main(int argc, char** argv){
    SGV sgv = parse_options(argv);

    sgv = main_loop(sgv);

    destroySGV(sgv);
    destroyConfig();

	return 0;
}
