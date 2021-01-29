#include "utilities.h"

CircularArray *circ;

int main(void){
	struct venda v;

	circ = initCA(4096);

	/* Read cicle */
	while(read(0, &v, sizeof(struct venda)) > 0){
		insertCAintoAG(circ, (int)v.code, v.total, (int)v.quantity);
	}

	flushCircularArray(circ, 0);

	freeCircularArray(circ);

	return 0;
}