#include <stdio.h>

void LerVetor (int* vet, int n){
	int i;
	for(i=0; i<n; i++){
		scanf("%d", &vet[i]);
	}
}

void printVetor (int* vet, int n){
	int i;
	for(i=0; i<n; i++){
		printf("%d\n", vet[i]);
	}
}

int main () {
	int n, i, j=0, k=0;
	scanf("%d", &n);
	int veta[n], vetb[n], vetx[2*n];
	LerVetor(veta, n);
	LerVetor(vetb, n);
	for(i=0; i<2*n; i++){
		if(i%2==0){
			vetx[i]=veta[j];
			j++;
		}
		else{
			vetx[i]=vetb[k];
			k++;
		}
	}
	printVetor(vetx, 2*n);
	return 0;
	}