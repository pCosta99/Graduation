#include <stdio.h>
#include <stdlib.h>
#include "fringeVetor.h"
#include "fringeOrd.h"
#include "fringeMin.h"
#include "findAndUnion.h"

#define NW       9
#define INFINITY 9999
#define BRANCO   0 // não visitado
#define CINZENTO 1 // em visita
#define PRETO    2 // visitado

#define VERDE    0
#define AZUL     1

// Algoritmos sobre grafos
typedef struct aresta {
	int dest;
	int peso;
	struct aresta *prox;
} *GrafoL[NW];

typedef int GrafoM[NW][NW];

void printArray (int ant[]) {
	for (int i = 0; i < NW; i++) 
		printf("%d ",ant[i]);
	puts("");
}

// Calcula o grau de entrada de um vértice
// T(V,E) = Ɵ(V+E)
int inDegreeL (GrafoL g, int v) {
	int r = 0;
	int i;
	struct aresta *pt;

	for (i = 0; i < NW; i++)
		for (pt = g[i]; pt != NULL; pt = pt->prox)
			if (pt->dest == v) r++;

	return r;
}

// Calcula o grau de entrada de um vértice
// T(V,E) = Ɵ(length g[v]) ??
int inDegreeM (GrafoM g, int v) {
	int r = 0;
	int i;

	for (i = 0; i < NW; i++)
		if (g[i][v]) r++;

	return r;  
}

// Calcula o grau de saída de um vértice
// T(V,E) = Ɵ(V)
int outDegreeL (GrafoL g, int v) {
	int r = 0;
	struct aresta *pt;

	for (pt = g[v]; pt != NULL; pt = pt->prox) r++;

	return r;
}

// Calcula o grau de saída de um vértice
// T(V,E) = Ɵ(V)
int outDegreeM (GrafoM g, int v) {
	int r = 0;
	int i;

	for (i = 0; i < NW; i++)
		if (g[v][i]) r++;

	return r;
}

// Calcula a capacidade de um vértice
// T(V,E) = Ɵ(V+E)
int capacidadeL (GrafoL g, int v) {
	int r = 0;
	int i;
	struct aresta *pt;

	for (i = 0; i < NW; i++) 
		for (pt = g[i]; pt != NULL; pt = pt->prox) {
			if (pt->dest == v) {r += pt->peso; break;}
			else if (i == v) r -= pt->peso;
		}

	return r;
}

// Calcula a capacidade de um vértice
// T(V,E) = Ɵ(V)
int capacidadeM (GrafoM g, int v) {
	int r = 0;
	int i;

	for (i = 0; i < NW; i++) 
		r = r - g[v][i] + g[i][v];

	return r;
}

// Calcula o vértice com maior capacidade dum grafo
// T(V,E) = Ɵ(V+E)
int maxCapL (GrafoL g) {
	int i, m, cap[NW];
	struct aresta *pt;


	for (i = 0; i < NW; i++) 
		for (pt = g[i]; pt != NULL; pt = pt->prox) {
			cap[i] -= pt->peso;
			cap[pt->dest] += pt->peso;
		}

	m = 0;
	for (i = 1; i < NW; i++)
		if (cap[i] > cap[m]) m = i;

	return m;
}

// Calcula o vértice com maior capacidade dum grafo
// T(V,E) = Ɵ(V^2)
int maxCapG (GrafoM g) {
	int i, m, cap[NW];

	for (i = 0; i < NW; i++)
		cap[i] = capacidadeM(g, i);

	m = 0;
	for (i = 1; i < NW; i++)
		if (cap[i] > cap[m]) m = i;

	return m;
}

// Indica se uma dada coloroção é válida para um grafo
// T(V,E) = Ɵ(V+E)
int colorOK (GrafoL g, int color[]) {
	int i, r = 1;
	struct aresta *pt;

	for (i = 0; i < NW && r; i++)
		for (pt = g[i]; pt != NULL && r; pt = pt->prox)
			if (color[i] == color[pt->dest]) r = 0;

	return r;
}

// Inverte um grafo
// T(N) = Ɵ(V+E)
void reverseGraphL (GrafoL g, GrafoL r) {
	int i;
	struct aresta *pt, *new;

	for (i = 0; i < NW; i++) r[i] = NULL;

	for (i = 0; i < NW; i++)
		for (pt = g[i]; pt != NULL; pt = pt->prox) {
			new = malloc(sizeof(struct aresta));
			new->dest = i;
			new->peso = pt->peso;
			new->prox = r[pt->dest];
			r[pt->dest] = new;
		}
}

// Inverte um grafo
// T(N) = Ɵ(V^2)
void reverseGraphM (GrafoM g, GrafoM r) {
	int i, j;

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			r[i][j] = 0;

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			if (g[i][j]) r[j][i] = g[i][j];
}

// Algoritmos sobre ordenação topológica
int dfirstTopSort (GrafoL g, int o, int color[], int *success, int seq[]) {
	struct aresta *pt;
	int t;
	color[o] = CINZENTO; // Em visita
	t = 0;
	for (pt = g[o]; pt != NULL; pt = pt->prox) 
		if (color[pt->dest] == BRANCO) 
			t += dfirstTopSort (g, pt->dest, color, success, seq+t);
		else if (color[pt->dest] == CINZENTO) 
			*success = 0;
	seq[t++] = o;
	color[o] = PRETO; // Visitado
	return t;
}

//Algoritmo de Tarjan (da sebenta)
// T(N) = Ɵ(V+E)
int tarjanTS (GrafoL g, int seq[]) {
	int color[NW];
	int v,j,t,r;
	for (v = 0; v < NW; v++) 
		color[v] = BRANCO;
	r = 1; t = 0;
	for (v = 0; v < NW; v++)
		if (color[v] == BRANCO)
			t += dfirstTopSort (g, v, color, &r, seq+t);
	//inverter sequência
	for (v = 0, j = t-1; v < j; v++,j--) 
		swap (seq, v, j);
	return r;
}

// Algoritmo de Khan (p.28 da Sebenta)
// T(N) = Ɵ(V+E)
int khanTS (GrafoL g, int seq[]) {
	int v, nAnt[NW];
	int inicio, fim;
	struct aresta *pt;

	for (v = 0; v < NW; v++)
		nAnt[v] = 0;

	// obter o grau de entrada de todos os vértices
	for (v = 0; v < NW; v++)
		for (pt = g[v]; pt != NULL; pt = pt->prox)
			nAnt[pt->dest]++;

	// inicializar a queue
	inicio = fim = 0; // vazia
	for (v = 0; v< NW; v++)
		if (nAnt[v] == 0) 
			seq[fim++] = v;

	while (inicio != fim) {
		v = seq[inicio++];
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			nAnt[pt->dest]--;
			if (nAnt[pt->dest] == 0) 
				seq[fim++] = pt->dest;
		}
	}

	return inicio;
}

// Determina se um grafo é acíclico
// T(N) = Ɵ(V+E)
int isAcyclic (GrafoL g) {
	int seq[NW];
	int r;

	r = khanTS(g, seq);

	if (r == NW) return 1; // acíclico
	else return 0; // cíclico
}

int depthFst (GrafoL g, int v, int N, int color[]) {
	int r = 0;
	struct aresta *pt;

	if (N == 0) return 0;

	color[v] = 2;

	for (pt = g[v]; pt != NULL; pt = pt->prox) 
		if (color[pt->dest] == 0) 
			r += 1 + depthFst(g, pt->dest, N-1, color);

	return r;
}

// Determina quantos vértices em g estão a uma distância menor que N de v
// Utiliza uma travessia depthFirst
int succN_DF (GrafoL g, int v, int N) {
	int r, i;
	int color[NW];

	for (i = 0; i < NW; i++)
		color[i] = BRANCO;

	r = depthFst(g, v, N, color);

	return r;
}

// Igual à de cima só que implementada com uma travessia breadth-first
int succN_BF (GrafoL g, int v, int N) {
	int orla[NW], visitados[NW], dist[NW]; 
	int inicio, fim;
	int i, r = 0;
	struct aresta *pt;

	for (i = 0; i < NW; i++) visitados[i] = BRANCO;

	inicio = fim = 0;
	orla[fim++] = v;
	visitados[v] = 2;
	dist[v] = 0;

	while (inicio != fim) {
		v = orla[inicio++];
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			if (visitados[pt->dest] == 0) {
				dist[pt->dest] = 1 + dist[v];
				visitados[pt->dest] = CINZENTO;
				orla[fim++] = pt->dest;
				if (dist[pt->dest] > N) return r;
				else r++;
			}
		}
	}

	return r;
}

// Indica se existe algum vértice que não é alcançável a partir de outro
int nonReachable (GrafoL g, int o) {
	int color[NW], orla[NW];
	struct aresta *pt;
	int v, inicio, fim, r = -1;

	for (v = 0; v < NW; v++) color[v] = BRANCO;

	inicio = fim = 0;
	orla[fim++] = o;
	color[o] = CINZENTO;

	while (inicio != fim) {
		v = orla[inicio++];
		color[v] = PRETO;
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			if (color[pt->dest] == 0) {
				color[pt->dest] = CINZENTO;
				orla[fim++] = pt->dest;
			}
		}
	}

	for (v = 0; v < NW; v++) 
		if (color[v] == BRANCO) 
			r = v;

	return r;
}

// Calcula se existe uma ligação entre v1 e v2 com custo superior a seccao
int ligacao (GrafoL g, int v1, int v2, int seccao) {
	int color[NW], orla[NW], dist[NW];
	struct aresta *pt;
	int v, inicio, fim;

	for (v = 0; v < NW; v++) color[v] = BRANCO;

	inicio = fim = 0;
	orla[fim++] = v1;
	color[v1] = CINZENTO;
	dist[v1] = 0;

	while (inicio != fim && v != v2) {
		v = orla[inicio++];
		color[v] = PRETO;
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			if (color[pt->dest] == 0) {
				dist[pt->dest] = pt->peso + dist[v];
				color[pt->dest] = CINZENTO;
				orla[fim++] = pt->dest;
			}
			else if (color[pt->dest] == 1)
				if (dist[pt->dest] < pt->peso + dist[v])
					dist[pt->dest] = pt->peso + dist[v];
		}
	}

	return (dist[v2] > seccao);
}

//Teste 2016. Bastante parecido com a funcao bicolor do Teste 2017
int bipartido (GrafoL g, int v1[]) {
	int v, i, r = 1, inicio, fim;
	int orla[NW], cor[NW];
	struct aresta *pt;

	for(i = 0; i < NW; i++) cor[i] = BRANCO; // nao visitado

	inicio = fim = 0;
	orla[fim++] = 0;
	v1[0] = AZUL;
	cor[0] = CINZENTO;

	while (inicio != fim && r) {
		v = orla[inicio++];
		cor[v] = PRETO;
		for (pt = g[v]; pt != NULL && r; pt = pt->prox) {
			if (cor[pt->dest] == BRANCO) {
				if (v1[v] == VERDE) v1[pt->dest] = AZUL;
				else  v1[pt->dest] = VERDE;
				orla[fim++] = pt->dest; 
				cor[pt->dest] = CINZENTO;
			}
			else if (v1[v] == v1[pt->dest]) r = 0;
		}
	}
	return r;
}

//Obtém todos os caminhos mais curtos a partir de um vértice até todos os outros
//Devolve o número de vértices que visitou (desnecessário)
// Utiliza um vetor desordenado como fronteira (orla)
int dijkstraSP (GrafoL g, int v, int pesos[], int ant[]) {
	int i;
	int r = 0; //número de vértices visitados
	int newcost, fringesize; 
	struct aresta *pt;
	int color[NW];
	FRINGE f = newFringe(NW);
	for (i = 0; i < NW; i++) color[i] = 0;

	fringesize = 0; //começar a orla (FRINGE)
	color[v] = CINZENTO; pesos[v] = 0; 
	addV(f, v, 0); fringesize++; //adicionar v à orla

	while (fringesize > 0) { // enquanto a orla não estiver vazia
		f = nextF(f, &v); //escolhe v da orla (vértice com menor custo até ao momento)
		fringesize--;
		r++;
		color[v] = PRETO; // v passa a visitado
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			newcost = pesos[v] + pt->peso; //novo custo descoberto para o vértice
			if ((color[pt->dest] == BRANCO) || ((color[pt->dest] == CINZENTO) && pesos[pt->dest] > newcost)) {
				ant[pt->dest] = v;
				pesos[pt->dest] = newcost; 
				if (color[pt->dest] == BRANCO) {
					addV(f, pt->dest, pesos[pt->dest]); // adicionar pt->dest à orla se este ainda não foi visitado
					fringesize++;
					color[pt->dest] = CINZENTO;
 				} else updateV(f, pt->dest, pesos[pt->dest]); //atualizar pt->dest na orla
			}
		}
	}
	return r;
}

// Utiliza um array ordenado como fronteira (orla)
int dijkstraSP_ORD (GrafoL g, int v, int pesos[], int ant[]) {
	int i;
	int r = 0; //número de vértices visitados
	int newcost, fringesize; 
	struct aresta *pt;
	int color[NW];
	FRINGEord f = newFringeOrd(NW);
	for (i = 0; i < NW; i++) color[i] = 0;

	fringesize = 0; //começar a orla (FRINGE)
	color[v] = CINZENTO; pesos[v] = 0; 
	f = addOrd(f, v, pesos); fringesize++; //adicionar v à orla

	while (fringesize > 0) { // enquanto a orla não estiver vazia
		f = nextOrd(f, &v); //escolhe v da orla (vértice com menor custo até ao momento)
		fringesize--;
		r++;
		color[v] = PRETO; // v passa a visitado
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			newcost = pesos[v] + pt->peso; //novo custo descoberto para o vértice
			if ((color[pt->dest] == BRANCO) || ((color[pt->dest] == CINZENTO) && pesos[pt->dest] > newcost)) {
				ant[pt->dest] = v;
				pesos[pt->dest] = newcost; 
				if (color[pt->dest] == BRANCO) {
					f = addOrd(f, pt->dest, pesos); // adicionar pt->dest à orla se este ainda não foi visitado
					fringesize++;
					color[pt->dest] = CINZENTO;
 				} else f = updateOrd(f, pt->dest, pesos); //atualizar pt->dest na orla
			}
		}
	}
	return r;
}

// Utiliza uma minHeap como fronteira (orla)
int dijkstraSP_MIN (GrafoL g, int v, int pesos[], int ant[]) {
	int i;
	int r = 0; //número de vértices visitados
	int newcost, fringesize; 
	struct aresta *pt;
	int color[NW];
	FRINGEmin f = newFringeMin(NW);
	for (i = 0; i < NW; i++) color[i] = 0;

	fringesize = 0; //começar a orla (FRINGE)
	color[v] = CINZENTO; pesos[v] = 0; 
	f = addMin(f, v, 0); fringesize++; //adicionar v à orla

	while (fringesize > 0) { // enquanto a orla não estiver vazia
		f = nextMin(f, &v); //escolhe v da orla (vértice com menor custo até ao momento)
		fringesize--;
		r++;
		color[v] = PRETO; // v passa a visitado
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			newcost = pesos[v] + pt->peso; //novo custo descoberto para o vértice
			if ((color[pt->dest] == BRANCO) || ((color[pt->dest] == CINZENTO) && pesos[pt->dest] > newcost)) {
				ant[pt->dest] = v;
				pesos[pt->dest] = newcost; 
				if (color[pt->dest] == BRANCO) {
					f = addMin(f, pt->dest, pesos[pt->dest]); // adicionar pt->dest à orla se este ainda não foi visitado
					fringesize++;
					color[pt->dest] = CINZENTO;
 				} else f = updateMin(f, pt->dest, pesos[pt->dest]); //atualizar pt->dest na orla
			}
		}
	}
	return r;
}

// Calcula uma árvore geradora de custo mínimo
// T(V,E) = Ɵ(V^2)
int primMST (GrafoL g, int ant[], int dist[]) {
	int fringesize, v, r = 0;
	int color[NW];
	struct aresta *pt;
	FRINGE f = newFringe(NW);

	for (v = 0; v < NW; ant[v++] = -1) color[v] = BRANCO;

	fringesize = 0;
	addV(f, 0, 0); fringesize++;
	dist[0] = 0;
	color[0] = CINZENTO;

	while (fringesize) {
		f = nextF(f, &v); fringesize--;
		r += dist[v];
		color[v] = PRETO;
		for (pt = g[v]; pt; pt = pt->prox) {
			switch (color[pt->dest]) {
				case BRANCO:
						color[pt->dest] = CINZENTO;
						fringesize++;
						addV(f, pt->dest, pt->peso);
						dist[pt->dest] = pt->peso;
						ant[pt->dest] = v;
						break;

				case CINZENTO: 
						if (dist[pt->dest] > pt->peso) {
							f = updateV(f, pt->dest, pt->peso);
							dist[pt->dest] = pt->peso;
							ant[pt->dest] = v;
						}
						break;
				default: break;
			}
		}
	}

	return r;
}

void sortedEdges (GrafoL g, EDGE e[], int N) {
	int v, used = 0;
	struct aresta *pt;
	struct sedge new;

	for (v = 0; v < N; v++) 
		e[v].cost = INFINITY;

	for (v = 0; v < NW; v++)
		for (pt = g[v]; pt; pt = pt->prox) {
			new.o = v;
			new.d = pt->dest;
			new.cost = pt->peso;
			used++;
			insert(e, new, used);
		}
}

void kruskalMST (GrafoL g, int ant[]) {
	int v, e1, o, d, arestas = 0;
	int pais[NW];
	struct aresta *pt;

	for (v = 0; v < NW; v++)
		for (pt = g[v]; pt; pt = pt->prox)
			arestas++;

	EDGE e[arestas];

	sortedEdges(g, e, arestas);
	initF(pais);
	initF(ant);

	for (e1 = 0, v = 0; e1 < NW-1; v++) {
		o = findF(pais,e[v].o);
		d = findF(pais,e[v].d);

		if (o != d) {
			unionF(pais, o, d);
			ant[e[v].d] = e[v].o;
			e1++;
		}
	}
} 

// T(V,E) = V^2+(V*(V+E)) = Ɵ(V^2+V*E) (depende do que for maior?)
void fechoTransitivo (GrafoL g, int pesos[NW][NW]) {
	int i, j, k = 0, v, inicio, fim;
	struct aresta *pt;
	int color[NW], orla[NW];

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			pesos[i][j] = 0;

	while (k < NW) {
		for (i = 0; i < NW; i++) color[i] = BRANCO;
		inicio = fim = 0;
		orla[fim++] = k;
		color[k] = CINZENTO;
		while (inicio != fim) {
			v = orla[inicio++];
			color[v] = PRETO;
			pesos[k][v] = 1;
			for (pt = g[v]; pt; pt = pt->prox) {
				if (color[pt->dest] == BRANCO) {
					orla[fim++] = pt->dest;
					color[pt->dest] = CINZENTO;
				}
			}
		}
		k++;
	}
}

// Calcula os caminhos mais curtos entre um vértice e todos os que ele alcança (pode ter pesos negativos)
// T(V,E) = Ɵ((V-1)* (V+E)) = Ɵ(V^2 + V*E)
// Retorna 0 se não houverem ciclos com peso negativo
int bellmanFordSP (GrafoL g, int o, int pesos[], int ant[]) {
	int i, v, newcost, r = 0;
	struct aresta *pt;

	for (i = 0; i < NW; ant[i++] = -1) 
		pesos[i] = INFINITY;

	pesos[o] = 0;

	for (i = 0; i < NW-1; i++)
		for (v = 0; v < NW; v++)
			if (pesos[v] != INFINITY)
				for (pt = g[v]; pt; pt = pt->prox) {
					newcost = pesos[v] + pt->peso;
					if (pesos[pt->dest] == INFINITY || newcost < pesos[pt->dest]) {
						pesos[pt->dest] = newcost;
						ant[pt->dest] = v;
					}
				}

	for (v = 0; v < NW && !r; v++) 
		for (pt = g[v]; pt && !r; pt = pt->prox)
			if (pt->peso + pesos[v] < pesos[pt->dest])
				r = 1;

	return r;
}

// Calcula o caminho mais longo de um grafo recorrendo à sua ordenação topológica
// T(V,E) = Ɵ(V+E)
int longestPath (GrafoL g, int ant[]) {
	int seq[NW], dist[NW], i, v, r;
	struct aresta *pt;

	khanTS(g, seq);

	for (i = 0; i < NW; ant[i++] = -1) 
		dist[i] = 0;

	for (i = 0; i < NW; i++) {
		v = seq[i];
		for (pt = g[v]; pt; pt = pt->prox) {
			if (dist[v] + pt->peso > dist[pt->dest]) {
				dist[pt->dest] = dist[v] + pt->peso;
				ant[pt->dest] = v;
			}
		}
	}

	r = 0;
	for (i = 1; i < NW; i++)
		if (dist[i] > dist[r]) r = i;

	return r;
}

// Calcula o caminho mais curto de um grafo recorrendo à sua ordenação topológica
// T(V,E) = Ɵ(V+E) 
int shortestPath (GrafoL g, int o, int d, int ant[]) {
	int seq[NW], dist[NW], i, v, start;
	struct aresta *pt;

	khanTS(g, seq);

	for (i = 0; i < NW; ant[i++] = -1) {
		dist[i] = INFINITY;
		if (seq[i] == o) start = i;
	}

	v = INFINITY;
	dist[o] = 0;

	for (i = start; i < NW && v != d; i++) {
		v = seq[i];
		for (pt = g[v]; pt && v != d; pt = pt->prox) {
			if (dist[v] + pt->peso < dist[pt->dest]) {
				dist[pt->dest] = dist[v] + pt->peso;
				ant[pt->dest] = v;
			}
		}
	}

	return dist[d];
}

// Calcula os caminhos mais curtos entre todos os vértices
// T(V,E) = Ɵ(V^3) -> Compensa em grafos muito densos
void floydWarshallL (GrafoL g, int pesos[NW][NW]) {
	int i, j, v;
	struct aresta *pt;

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			pesos[i][j] = 0;

	for (i = 0; i < NW; i++)
		for (pt = g[i]; pt; pt = pt->prox)
			pesos[i][pt->dest] = pt->peso;

	for (v = 0; v < NW; v++)
		// para todos os antecessores de v
		for (i = 0; i < NW; i++)
			if (pesos[i][v])
				for (j = 0; j < NW; j++)
					if (pesos[v][j])
						if (pesos[i][j] == 0 || pesos[i][j] > pesos[i][v] + pesos[v][j])
							pesos[i][j] = pesos[i][v] + pesos[v][j];
}

// T(V,E) = Ɵ(V^3)
void floydWarshallM (GrafoM g, int pesos[NW][NW]) {
	int i, j, v;

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			pesos[i][j] = g[i][j];

	for (v = 0; v < NW; v++)
		for (i = 0; i < NW; i++)
			if (pesos[i][v])
				for (j = 0; j < NW; j++)
					if (pesos[v][j])
						if (pesos[i][j] == 0 || pesos[i][j] > pesos[i][v] + pesos[v][j])
							pesos[i][j] = pesos[i][v] + pesos[v][j];
}

// Teste 2015 Ex.3
int quantosDepois(GrafoL g, int v, int dist) {
	int pais[NW], pesos[NW];
	int i, r = 0;

	for(i = 0; i < NW; pais[i++] = -1) pesos[i] = -1;
	dijkstraSP(g, v, pesos, pais);
	for(i = 0; i < NW; i++)
		if (pesos[i] > dist) 
			r++;

	return r;
}

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

// Matriz de adjacência para lista de adjacência
// T(N) = Ɵ(V^2)
void matParaLista (GrafoM a, GrafoL b) {
	int i, j; struct aresta *pt;
	for(i = 0; i < NW; i++) b[i] = NULL;
		for (i = 0; i < NW; i++)
			for (j = NW-1; j >= 0; j--) 
				if (a[i][j]) {
					pt = malloc(sizeof(struct aresta));
					pt->dest = j;
					pt->prox = b[i];
					b[i] = pt;
				}
}

// Lista de adjacência para matriz de adjacência
// T(N) = Ɵ(V^2)
void listaParaMat (GrafoL a, GrafoM b) {
	int i, j;
	struct aresta *pt;

	for (i = 0; i < NW; i++)
		for (j = 0; j < NW; j++)
			b[i][j] = 0;

	for (i = 0; i < NW; i++)
		for (pt = a[i]; pt != NULL; pt = pt->prox)
			b[i][pt->dest] = pt->peso;
}

// Inverte um nodo de um grafo
void reverseLL (struct aresta **pt) {
	struct aresta *current = *pt;
	struct aresta *prev = NULL, *next = NULL;

	while (current != NULL) {
		next = current->prox;
		current->prox = prev;
		prev = current;
		current = next;
	}
	*pt = prev;
}

// Inverte todos os nodos de um grafo
void fixGraphL (GrafoL g) {
	for (int v = 0; v < NW; v++) 
		reverseLL(&g[v]);
}

// Insere um nodo num grafo.
void insertL (GrafoL b, int or, int p, int dest) {
	struct aresta *pt;
	pt = malloc(sizeof(struct aresta));
	pt->dest = dest;
	pt->peso = p;
	pt->prox = b[or];
	b[or] = pt;
}

// Travessia Breadth First (em largura)
void BFS (GrafoL g, int o, int dist[]) {
	int color[NW], orla[NW]; // queue
	int inicio, fim; // inteiros para controlar a queue
	int v; // vertice em execuçao
	struct aresta *pt;
	
	for (v = 0; v < NW; v++) color[v] = BRANCO;
	
	inicio = fim = 0;
	orla[fim++] = o;
	dist[o] = 0;
	color[o] = CINZENTO;

	while (inicio != fim) {
		v = orla[inicio++];
		color[v] = PRETO;
		for (pt = g[v]; pt != NULL; pt = pt->prox) {
			if (color[pt->dest] == BRANCO) {
				dist[pt->dest] = 1 + dist[v];
				orla[fim++] = pt->dest;
			}
		}
	}
}

//Ponto de encontro mais barato entre ambos os nodos
int pEncontro (GrafoL g, int v1, int v2) {
	int d1[NW], d2[NW];
	int i, m = INFINITY, r = -1;

	for(i = 0; i < NW; i++) d1[i] = d2[i] = -1;

	BFS(g, v1, d1);
	BFS(g, v2, d2);

	for(i = 0; i < NW; i++) 
		if (d1[i] != -1 && d2[i] != -1 && d1[i]+d2[i] < m) {
			m = d1[i] + d2[i];
			r = i;
		}

	return r;
}
// Verifica se o array é um homomorfismo entre a e b (função de teste)
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

// Determina se uma coloração é válida para um grafo (função de teste)
// T(V,E) = O(V + E)
int colorOk (GrafoL g, int color[]) {
	int v, r = 1;
	struct aresta *pt;

	for (v = 0; v < NW && r; v++) 
		for (pt = g[v]; pt && r; pt = pt->prox) 
			if (color[v] == color[pt->dest]) 
				r = 0;

	return r;
}

int kColorRec (GrafoL g, int color[], int k, int i) {
	int v;

	if (i == NW) return (colorOk(g, color));
	else for (v = 0; v < k; v++) {
		color[i] = v;
		if (kColorRec(g, color, k, i+1)) 
			return 1;
	}

	return 0;
}

// PC: T(V,E) = Ɵ(k^V)
// MC: T(V,E) = Ɵ(V+E)
int kColor (GrafoL g, int k) {
	int v;
	int color[NW];

	for (v = 0; v < NW; v++)
		color[v] = 0;

	return (kColorRec(g, color, k, 0));
}

// Exame 2015 2016

int isThereAPathRec (GrafoL g, int dist[], int w, int start) {
	struct aresta *pt;

	for (pt = g[start]; pt; pt = pt->prox) {
		dist[pt->dest] = dist[start] + pt->peso;
		if (dist[pt->dest] == w || isThereAPathRec(g, dist, w, pt->dest)) return 1;
	}

	return 0;
}

int isThereAPath (GrafoL g, int w) {
	int dist[NW];
	int v;

	for (v = 0; v < NW; v++) {
		dist[v] = 0;
		if (isThereAPathRec(g, dist, w, v))
			return 1;
	}

	return 0;
}

int main (void) {
	//GrafoL *g = malloc(sizeof(GrafoL));

	// Para inserir elementos usem a função insertL().
	// Exemplo: insertL(*g, 0, 4, 2) cria uma aresta de 0 para 2 com peso 4.
	// Alterem o NW (no define) consoante o número de vértices que tiverem. 
	// Têm também de alterar NW no ficheiro findAndUnion.h

	return 0;
}
