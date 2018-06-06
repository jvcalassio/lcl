#include <stdio.h>
 
 /*le os elementos de um vetor*/
void scanvet (int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
}

/*mostrar elementos de um vet*/
void printvet(int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		printf("%d\n", vet[i]);
}

/*cria vetores de uniao e interseção entre dois vetores*/
void interuni(int* x, int* y, int n){
 	int i, j, in=0, un=0, k=0, achou;
 	for(i=0; i<n; i++){
 		for(j=0; j<n; j++){
 			if(x[i]==y[j])
 				in++;
 		}
 	}
	un=(2*n)-in;
	int uniao[un], inter[in];
	while(k<in){
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				if(x[i]==y[j]){
					inter[k]=x[i];
					k++;
				}
			}
		}
	}
	k=0;
	while(k<un){
		if(k<n){
			uniao[k]=y[k];
			k++;
		}
		else{
			for(i=0; i<n; i++){
				achou=0;
				for(j=0; j<n && achou!=1; j++){
					if(x[i]==y[j])
						achou=1;
				}
				if(achou==0){
					uniao[k]=x[i];
					k++;
				}
			}
		}
	}
	printf("uniao:\n");
	printvet(uniao, un);
	printf("intersecao:\n");
	printvet(inter, in);
}

int main () {
	int n;
	scanf("%d", &n);
	int x[n], y[n];
	scanvet(x, n);
	scanvet(y, n);
	interuni(x, y, n);
	return 0;
}