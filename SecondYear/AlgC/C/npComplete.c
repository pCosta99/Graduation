#include "npComplete.h"

// Sub Set Sum

int subSetZero (int num[], int sub[], int N) {
	int r = 0, testAllZero = 0, i;

	for (i = 0; i < N; i++) {
		if (sub[i] == 1) testAllZero = 1;
		r += num[i] * sub[i];
	}

	return (r == 0 && testAllZero);
}

int subSetSumRec (int num[], int sub[], int N, int k) {
	int i;

	if (k == N) return (subSetZero(num, sub, N));
	else for (i = 0; i <= 1; i++) {
		sub[k] = i; 
		if (subSetSumRec(num, sub, N, k+1))
			return 1;
	}

	return 0;
}

int subSetSum (int num[], int N) {
	int i;
	int sub[N];

	for (i = 0; i < N; i++)
		sub[i] = 1;

	return (subSetSumRec(num, sub, N, 0));
}

// Teste 2 2015/2016

int subSetHalfTest (int num[], int sub[], int N, int halfNum) {
	int i, r = 0;

	for (i = 0; i < N; i++) 
		r += num[i] * sub[i];

	return (r == halfNum);
}

int subSetHalfRec (int num[], int sub[], int halfNum, int N, int k) {
	int i;

	if (k == N) return subSetHalfTest(num, sub, N, halfNum);
	else for (i = 0; i <= 1; i++) {
		sub[k] = i;
		if (subSetHalfRec(num, sub, halfNum, N, k+1))
			return 1;
	}

	return 0;
}

int subSetHalf (int num[], int N) {
	int halfNum;
	int sub[N];
	int i;

	for (i = 0; i < N; i++) {
		halfNum += num[i];
		sub[i] = 1;
	}

	halfNum = halfNum/2;

	return (subSetHalfRec(num, sub, halfNum, N, 0));
}

int main (void) {
	int N = 10;
	int num[] = {7,2,4,11,1,2,3,1,2,9};

	printf("%d\n", subSetSum(num, N));
	printf("%d\n", subSetHalf(num, N));

	return 0;
}
