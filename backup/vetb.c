#include <stdio.h>
/*le os elementos do vetor*/
void LerVetor(int* vet, int n){
	int i;
	for(i=0; i<n; i++){
		scanf("%d", &vet[i]);
	}
}
/*troca elementos negativos de um vet por 0*/
void trocarzero(int *vet, int n){
	int i;
	for(i=0; i<n; i++){
		if(vet[i]<0)
			vet[i]=0;
	}
}
/*imprime os elementos de um vet*/
void printVetor(int* vet, int n){
	int i;
	for(i=0; i<n; i++){
		printf("%d\n", vet[i]);
	}
}

int main () {
	int n;
	scanf("%d", &n);
	int vet[n];
	LerVetor(vet, n);
	trocarzero(vet, n);
	printVetor(vet, n);
return 0;
}