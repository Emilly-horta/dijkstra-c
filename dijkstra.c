#include<stdio.h>
#include<stdlib.h>

#define Nodo int

/*
 * Matriz de Adjacência 
 */
struct rede{
	int nodos;
	int enlaces;
	int **adj;
};

typedef struct rede *Rede;

int **matrizIni(int r, int c, int val){
	Nodo i, j;
	int **m = malloc(r * sizeof (int*));
	for(i = 0; i < r; i++){
		m[i] = malloc(c * sizeof(int));
	}
	
	for(i = 0; i < r; i++){
		for(j = 0; j < c; j++){
			m[i][j] = val;
		}
	}
	
	return m;
}

Rede redeInicializa(int v){
	Rede r = malloc(sizeof *r);
	r->nodos = v;
	r->enlaces = 0;
	r->adj = matrizIni(v,v,0);
	return r;	
}

void redeInsere(Rede r, Nodo v, Nodo w, int custo){
	if(r->adj[v][w] == 0){
		r->adj[v][w] = custo;
		r->enlaces++;
	}
}

void redeRemove(Rede r, Nodo v, Nodo w){
	if(r->adj[v][w] > 0){
		r->adj[v][w] = 0;
		r->enlaces--;
	}
}

void redeMostra(Rede r){
	Nodo v, w;
	for(v = 0; v < r->nodos; v++ ){
		printf("%2d -> ", v);
		for(w = 0; w < r->nodos; w++){
			if(r->adj[v][w] > 0){
				printf("%2d", w);
			}
		}
		printf("\n");
	}
}

void inicializaCaminho(Nodo *caminho, int tamanho){
	int i;
	for(i = 0; i < tamanho; i++)
		caminho[i] = -1;
}

void mostraCaminho(Nodo *caminho, int tamanho){
	int i;
	for(i = 0; i < tamanho-1; i++)
		if(caminho[i] == -1)
			break;
		else
			printf("%d ", caminho[i]);
	
}

struct rota{
	int custo;
	int saltos;
};

struct rota *rotas;
int qtdRotas;

int mostraCusto(Rede r, Nodo *rota, int nivel){
	int i = 0;
	int x, y, custo = 0;
	if(nivel == 0){
		return 0;
	}
	do{
		x = rota[i++];
		y = rota[i];
		custo += r->adj[x][y];
	}while(i < nivel);
	return custo;
}

void mostraRota(Rede r, Nodo *rota, int nivel){
	int i;
	printf("%d: ", qtdRotas+1);
	for(i = 0; i < nivel; i++){
		printf("%d ", rota[i]);		
	}
	rotas[qtdRotas].custo = mostraCusto(r, rota, nivel);
	rotas[qtdRotas].saltos = nivel-1;
	printf(" - Custo: %d", rotas[qtdRotas].custo);
	printf(" - Saltos: %d\n", rotas[qtdRotas].saltos);
	qtdRotas++;
}

struct Estado{
	int antecessor;
	int tamanho;
	enum{permanente, tentativa} rotulo;
};

void dijkstra(Rede r, Nodo s , Nodo t , int *caminho){
	
	struct Estado *estado = malloc(r->nodos * sizeof(struct Estado));
 	
	int i, k, min;
 	
	for (i = 0; i < r->nodos; i++){
		estado[i].antecessor =-1;
		estado[i].tamanho = 1000;
		estado[i].rotulo = tentativa;
	}

	estado[t].tamanho = 0;
	estado[t].rotulo = permanente;
	k = t;
	do{
		for (i = 0; i < r->nodos ;i++){
			if(r->adj[k][i] != 0 && estado[i].rotulo == tentativa){
				if(estado[k].tamanho + r->adj[k][i] < estado[i].tamanho){
					estado[i].antecessor = k;
					estado[i].tamanho = estado[k].tamanho + r->adj[k][i];
 				}
 			}
		}
 		
		k = 0;
 		min = 1000;
 		
 		for(i = 0 ;i <  r->nodos; i++){
 			if(estado[i].rotulo == tentativa && estado[i].tamanho < min  ){
 				min = estado[i].tamanho;
 				k = i;
 			}
 		}
 		
 		estado[k].rotulo = permanente;
 		
 	}while(k != s);
 	
 	i = 0 , k = s; 	
 	do{
 	 	caminho[i++] = k ;
 		k = estado[k].antecessor;
 	 }while(k >= 0);
 }
 
int verificaPercorrido(int *rota, int nodo, int nivel){
	int i;
	for( i = 0; i < nivel; i++){
		if(rota[i] == nodo)
			return 1;
	}
	return 0;
}
 
void mostraRotas(Rede r, Nodo v, Nodo w, Nodo *a, int nivel){
	if(v == w){
		mostraRota(r, a, nivel);
		return;
	}else {
		if(nivel == 0){
			a[0] = v;
			nivel++;
		}
	
		int i;
		for(i = 0; i < r->nodos; i++){
			if(r->adj[v][i] > 0 && ( verificaPercorrido(a, i, nivel) != 1)){
				a[nivel] = i;
				mostraRotas(r, i, w, a, nivel + 1);
			}
		}
	}
}

int main(){
	
	Nodo origem;
	Nodo destino;
	
	int nodos = 5;
	int enlaces = 6;
	//~ 
	//~ printf("Nodos:");
	//~ scanf("%d", &enlaces);
	//~ printf("Enlaces:");
	//~ scanf("%d", &enlaces);
	
	Rede nova = redeInicializa(nodos);
	redeInsere(nova, 0, 1, 1);
	redeInsere(nova, 0, 2, 2);
	redeInsere(nova, 1, 0, 1);
	redeInsere(nova, 1, 3, 2);
	redeInsere(nova, 1, 4, 3);
	redeInsere(nova, 2, 0, 2);
	redeInsere(nova, 2, 3, 1);
	redeInsere(nova, 3, 1, 2);
	redeInsere(nova, 3, 2, 1);
	redeInsere(nova, 3, 4, 1);
	redeInsere(nova, 4, 1, 3);
	redeInsere(nova, 4, 3, 1);
	
	//~ redeInsere(nova, 0, 1, 1);
	//~ redeInsere(nova, 0, 2, 3);
	//~ redeInsere(nova, 0, 3, 1);
	//~ redeInsere(nova, 1, 0, 1);
	//~ redeInsere(nova, 1, 2, 2);
	//~ redeInsere(nova, 2, 0, 3);
	//~ redeInsere(nova, 2, 1, 2);
	//~ redeInsere(nova, 2, 3, 1);
	//~ redeInsere(nova, 3, 0, 1);
	//~ redeInsere(nova, 3, 2, 1);
		
	redeMostra(nova);
	
	printf("Informe uma ORIGEM e DESTINO válidos!\n");
	
	do{
		printf("Origem:");
		scanf("%d", &origem);
	}while(origem >= nova->nodos);
	
	do{
		printf("Destino:");
		scanf("%d", &destino);
	}while(destino >= nova->nodos);
	
	rotas = malloc(enlaces * enlaces * sizeof(struct rota));
	qtdRotas = 0;
	
	Nodo *a = malloc(nodos * sizeof(int));
	inicializaCaminho(a, nodos);
	printf("--------Todas as rotas--------\n");
	mostraRotas(nova, origem, destino, a, 0);
	
	
	int i, menorCusto = rotas[0].custo, menorSalto = rotas[0].saltos, melhorRotaCusto = 0, melhorRotaSalto = 0;
	for(i = 1; i < qtdRotas; i++){
		if(rotas[i].custo < menorCusto){
			menorCusto = rotas[i].custo;
			melhorRotaCusto = i;
		}
		if(rotas[i].saltos < menorSalto){
			menorSalto = rotas[i].saltos;
			melhorRotaSalto = i;
		}
	}
	printf("\nRota com menor custo: %d\n", melhorRotaCusto+1);
	printf("Menor custo: %d\n", menorCusto);
	printf("\nRota poucos saltos: %d\n", melhorRotaSalto+1);
	printf("Poucos saltos: %d\n", menorSalto);
	
	Nodo *caminho = malloc(nodos * sizeof(int));
	inicializaCaminho(caminho, nodos);
	
	dijkstra(nova, origem, destino, caminho);	
	printf("\n--------Melhor Rota - Dijkstra--------\n");
	mostraCaminho(caminho, nodos);		
	
	return 0;
}
