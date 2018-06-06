#include <stdio.h>
 
 /*le os elementos de um vetor*/
void scanvet (int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
}

/*mostrar elementos de um vet*/
void printvet (int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		printf("%d\n", vet[i]);
}

/*retira zeros de um vetor*/
void retirazero (int* vet, int n){
	int i, j, cont=0, aux;
	for(i=0; i<n; i++){
		if(vet[i]==0)
			cont++;
	}
	for(i=0; i<n; i++){
		if(vet[i]==0){
			j=i;
			while(vet[j]==0){
				j++;
			}
			aux = vet[i];
			vet[i]=vet[j];
			vet[j]=aux;
		}
	}
	for(i=n-cont; i<n; i++)
		vet[i]=-1;
}

int main () {
	int n;
	scanf("%d", &n);
	int vet[n];
	scanvet(vet, n);
	retirazero(vet, n);
	printvet(vet, n);
	return 0;
}