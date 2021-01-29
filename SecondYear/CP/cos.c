#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double myCos(double x, int n){
	double c=1,g=12,k=18;double f=(-(pow(x,2)/2));int i;
	for(i=0; i<n+1; i++) {c+=f;f*=(-(pow(x,2)/g));g+=k,k+=8;}
	return c;
}

int main(void){
	int i = 10;
	printf("Valor: %f\nIterações: %d\nResultado: %f\nValor real: %f\n", M_PI, i, myCos(M_PI, i), cos(M_PI));
}