#include <stdio.h>

/*le os elementos de um vet*/
void LerVetor (int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
}
/*mostra os elementos de um vet*/
void mostrarVetor(int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		printf("%d\n", vet[i]);
}
/*Inverte os termos de um vetor*/
void InverterVetor (int* vet, int n){
	int i, veti[n];
	for(i=0; i<n; i++)
		veti[i]=vet[n-i-1];
	for(i=0; i<n; i++)
		vet[i]=veti[i];
}
int main() {
	int n;
	scanf("%d", &n);
	int vet[n];
	LerVetor(vet, n);
	InverterVetor(vet, n);
	mostrarVetor(vet, n);
	return 0;
}