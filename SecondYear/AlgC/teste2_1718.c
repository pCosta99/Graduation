// Exercicio 1

3 8 25 9 24 32 37 110 120 31
8 9 25 31 24 32 37 110 120
9 24 25 31 120 32 37 110 
24 31 25 110 120 32 37
25 31 32 110 120 37
31 37 32 110 120
32 37 120 110
37 110 120
110 120
120 
Vazio

// Exercicio 2

// Insere um elemento numa tabela de hash
// Melhor caso: T(N) = Ɵ(1)
// Pior caso: T(N) = Ɵ(N)
int update (HashTable h, int key, int value) {
	int p = hash(key), probe = 0;

	while (h[p].probeC >= 0 && probe < HASHSIZE_PC && h[p].key != key) {
		p = (p+1) % HASHSIZE_PC;
		probe++;
	}

	if (probe == HASHSIZE_PC) return 2; //overflow (deu a volta completa a tabela)
	if (h[p].probeC >= 0) {
		h[p].value = value;
		return 1; //existe 
	}
	h[p].key = key;
	h[p].value = value;
	h[p].probeC = probe;

	return 0; //Elemento inserido
}

// Retorna 1 se há elementos repetidos no array
// A tabela de hash deve ter, no mínimo, o dobro do tamanho do array inserido (2*N).
// T(N) = Ɵ(N)
int repetidos (int v[], int N) {
	HashTable h;
	int i, r = 0;

	init(h);

	for (i = 0; i < N && r != 1; i++) 
		r = update(h, v[i], v[i]);

	return r;
}

// Exercicio 3
//Pergunta 3 Teste 17/18 AlgC
// T(V,E) = O(V+E)
int travessiaBF (GrafoL g, int or) {
	int cor[NW], orla[NW];
	int inicio,fim;
	int v, r = 1;
	struct aresta *pt;
	inicio = fim = 0;

	for (int i = 0; i < NW; i++) cor[i] = BRANCO;

	orla[fim++] = or;
	cor[or] = CINZENTO;
	while (inicio != fim) {
		v = orla[inicio++];
		cor[v] = PRETO;
		for (pt = g[v]; pt != NULL && r; pt = pt->prox) {
			if (cor[pt->dest] == BRANCO) {
				orla[fim++] = pt->dest;
				cor[pt->dest] = CINZENTO;
			}
			else r = 0;
		}
	}
	return r;
}

// Indica se um grafo é uma árvore (Pergunta 3 Teste 2018)
// T(V,E) = O(V+E)
int isTree (GrafoL g) {
	struct aresta *pt;
	int i, raiz = -1;
	int arr[NW];

	for (i = 0; i < NW; i++) arr[i] = 0;

	for (i = 0; i < NW; i++) 
		for (pt = g[i]; pt != NULL; pt = pt->prox)
			arr[pt->dest]++;

	for (i = 0; i < NW; i++) {
		if (arr[i]) ;
		else if (raiz == -1) raiz = i;
		else return -1;
	}

	if (travessiaBF(g, raiz)) return raiz;
	else return -1;
}

// Exercicio 4

/ Verifica se o array é um homomorfismo entre a e b (função de teste)
// Fase determinística do algoritmo
// T(V,E) = O(V + E ) 
int testHomomorphic (GrafoL a, GrafoL b, int arr[]) {
	int v, trigger = 1;
	struct aresta *pt, *x;

	for (v = 0; v < NW && trigger; v++)
		for (pt = a[v]; pt && trigger; pt = pt->prox) {
			trigger = 0;
			for (x = b[arr[v]]; x; x = x->prox) {
				if (x->dest == arr[pt->dest]) trigger = 1;
			}
		}

	return trigger;
}

// Fase não-determinística (gera todas as tentativas possíveis)
int homomorphicRec (GrafoL a, GrafoL b, int arr[], int i) {
	int v;

	if (i == NW) return (testHomomorphic(a,b, arr));

	else for (v = 0; v < NW; v++) {
		arr[i] = v;
		if (homomorphicRec(a, b, arr, i+1))
			return 1;
	}

	return 0;
}

// Testa se existe um homomorfismo entre dois grafos
int homomorphic (GrafoL a, GrafoL b) {
	int arr[NW];

	for (int v = 0; v < NW; v++) arr[v] = 0;

	return (homomorphicRec(a, b, arr, 0));
}

// Enjoy =)
