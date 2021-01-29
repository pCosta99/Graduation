#include "utilities.h"

/* Small program to print any of the binary files */

/* Vendas printer */
void printVendasPathed(char* path){
	int fdv = open(path, O_RDONLY);
	struct venda v;

	/* Read and print */
	while(read(fdv, &v, sizeof(struct venda))){
		readVenda(v);
	}

	/* Close descriptors */
	close(fdv);
}

/* Vendas printer */
void printVendas(void){
	int fdv = open(VENDAS, O_RDONLY);
	struct venda v;

	/* Skip accumulator */
	lseek(fdv, sizeof(int32_t), SEEK_SET);

	/* Read and print */
	while(read(fdv, &v, sizeof(struct venda))){
		readVenda(v);
	}

	/* Close descriptors */
	close(fdv);
}

int main(int argc, char** argv){
	printVendasPathed(argv[1]);
	return 0;
}