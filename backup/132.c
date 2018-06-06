#include <stdio.h>

/*le um vetor*/
void scanvetor (int vet[], int n){
	int i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
}

/*printa um vetor em formato de conjunto*/
void printconjunto(int vet[], int n){
	int i;
	printf("{");
	for(i=0; i<n; i++){
		if(i!=(n-1))
			printf("%d,", vet[i]);
		else
			printf("%d", vet[i]);
	}
	printf("}\n");
}

/*ordena um vetor*/
void bubblesort(int vet[], int n){
	int i, j, ordenado=0;
	for(i=0; i<n && ordenado==0; i++){
		ordenado=1;
		for(j=0; j<n-1-i; j++){
			if(vet[j]>vet[j+1]){
				ordenado=0;
				int aux=vet[j];
				vet[j]=vet[j+1];
				vet[j+1]=aux;
			}
		}
	}
}

/*cria os vetores pikachu(uniao) e chikorita(intersecao)*/
void pokemon(int a[], int n, int b[], int m){
	int i, chi=0, pi=0, l, r, mid, achou, j, k=0;
	for(i=0; i<n; i++){
		achou=0; r=m-1; l=0;
		while(l<=r && achou==0){
			mid=(l+r)/2;
			if(a[i]>b[mid])
				l=mid+1;
			else if(a[i]<b[mid])
				r=mid-1;
			else{
				achou=1;
				chi++;
			}
		}
	}
	pi=m+n-chi;
	int pikachu[pi], chikorita[chi];
	for(i=0; i<m; i++)
		pikachu[i]=b[i];
	for(j=0; j<n; j++){
		achou=0; r=m-1; l=0;
		while(l<=r && achou==0){
			mid=(l+r)/2;
			if(a[j]>b[mid])
				l=mid+1;
			else if(a[j]<b[mid])
				r=mid-1;
			else
				achou=1;
		}
		if(achou==0){
			pikachu[i]=a[j];
			i++;
		}
		else if(achou == 1){
			chikorita[k]=a[j];
			k++;
		}
	}
	bubblesort(pikachu, pi);
	bubblesort(chikorita, chi);
	printf("pikachu: ");
	printconjunto(pikachu, pi);
	printf("chikorita: ");
	printconjunto(chikorita, chi);
}


int main () {
	int n, m;
	scanf("%d %d", &n, &m);
	int a[n], b[m];
	scanvetor(a, n);
	bubblesort(a, n);
	scanvetor(b, m);
	bubblesort(b, m);
	pokemon(a, n, b, m);	
	return 0;
}