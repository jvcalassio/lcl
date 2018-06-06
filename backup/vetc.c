#include <stdio.h>
/*le dois vetores*/
void ler2vetores(int* veta, int* vetb, int n){
	int i;
	for(i=0; i<n; i++){
		scanf("%d", &veta[i]);
		scanf("%d", &vetb[i]);
	}
}
/*busca um codigo e retorna um preço*/
void busca(int x, int* cod, int* preco, int n){
	int i=0;
	while(cod[i]!=x)
		i++;
	printf("%d\n", preco[i]);
}

int main () {
	int n, x;
	scanf("%d", &n);
	int cod[n], preco[n];
	ler2vetores(cod, preco, n);
	scanf("%d", &x);
	busca(x, cod, preco, n);
return 0;
}